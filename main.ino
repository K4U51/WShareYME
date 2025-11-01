#include "Wireless.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "BAT_Driver.h"

//--------------------------------------
// Overlay movement task (16x16 overlay, auto-calibration)
//--------------------------------------
void Lvgl_UpdateOverlayFromGyro(void * parameter)
{
    // --- Screen & overlay geometry ---
    const int16_t SCREEN_CENTER_X = 120; // 240x240 display center
    const int16_t SCREEN_CENTER_Y = 120;
    const int16_t OVERLAY_HALF_W  = 9;   // 16x16 image -> half = 8
    const int16_t OVERLAY_HALF_H  = 9;
    const float   sensitivity     = 40.0f; // adjust for desired motion range

    // --- Step 1: auto-calibration ---
    float offsetX = 0.0f, offsetY = 0.0f;
    const int samples = 100;
    Serial.println("[Overlay] Calibrating... hold board level");

    for (int i = 0; i < samples; i++) {
        offsetX += getAccX();
        offsetY += getAccY();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    offsetX /= samples;
    offsetY /= samples;

    Serial.print("[Overlay] Calibration complete. offsetX=");
    Serial.print(offsetX, 3);
    Serial.print(" offsetY=");
    Serial.println(offsetY, 3);

    // --- Step 2: main overlay loop ---
    while (1)
    {
        float ax = getAccX() - offsetX;
        float ay = getAccY() - offsetY;

        ax = constrain(ax, -1.0f, 1.0f);
        ay = constrain(ay, -1.0f, 1.0f);

        // Position top-left of overlay so its visual center is screen center
        int16_t x = (int16_t)(ax * sensitivity) + SCREEN_CENTER_X - OVERLAY_HALF_W;
        int16_t y = (int16_t)(-ay * sensitivity) + SCREEN_CENTER_Y - OVERLAY_HALF_H;

        Lvgl_MoveOverlay(x, y);
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

//--------------------------------------
// Driver task (sensors, battery, etc.)
//--------------------------------------
void Driver_Loop(void *parameter)
{
    while (1)
    {
        QMI8658_Loop();
        RTC_Loop();
        BAT_Get_Volts();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Driver_Init()
{
    Flash_test();
    BAT_Init();
    I2C_Init();
    TCA9554PWR_Init(0x00);
    Set_EXIO(EXIO_PIN8, Low);
    PCF85063_Init();
    QMI8658_Init();

    // Background driver task
    xTaskCreatePinnedToCore(
        Driver_Loop,
        "Other Driver task",
        4096,
        NULL,
        3,
        NULL,
        0
    );

    // Overlay movement task
    xTaskCreatePinnedToCore(
        Lvgl_UpdateOverlayFromGyro,
        "Overlay Task",
        4096,  // increased stack for floating-point math
        NULL,
        4,
        NULL,
        1
    );
}

//--------------------------------------
// Setup
//--------------------------------------
void setup()
{
    Wireless_Test2();
    Driver_Init();

    LCD_Init();    // Initialize SPI display
    SD_Init();     // Must be after LCD

    Lvgl_Init();   // Initialize LVGL library
    Lvgl_Example1(); // Load background and overlay
}

//--------------------------------------
// Main loop
//--------------------------------------
void loop()
{
    static unsigned long startTime = millis();
    static bool bgSwitched = false;

    Lvgl_Loop();  // LVGL handler (must run frequently)

    // Automatically switch to bg2 after 1 second
    if (!bgSwitched && millis() - startTime >= 1000)
    {
        Lvgl_SwitchBackground();
        bgSwitched = true;
    }

    vTaskDelay(pdMS_TO_TICKS(5));
}
