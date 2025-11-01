//updates axis for wall/auto mount.

#include "Wireless.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "BAT_Driver.h"

//--------------------------------------
// Overlay movement task (auto-calibration, wall mount)
//--------------------------------------
void Lvgl_UpdateOverlayFromGyro(void * parameter)
{
    const float sensitivity = 40.0f; // adjust for desired motion range
    const int samples = 100;

    // --- Step 1: auto-calibration ---
    float offsetX = 0.0f, offsetY = 0.0f;
    Serial.println("[Overlay] Calibrating... hold board level");

    for (int i = 0; i < samples; i++) {
        offsetX += getAccZ(); // horizontal wall mount
        offsetY += getAccY(); // vertical
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
        float ax = getAccZ() - offsetX; // horizontal
        float ay = getAccY() - offsetY; // vertical

        ax = constrain(ax, -1.0f, 1.0f);
        ay = constrain(ay, -1.0f, 1.0f);

        // Move overlay relative to background g_img2
        if (g_img3 && g_img2) {
            lv_coord_t dx = (lv_coord_t)(ax * sensitivity);
            lv_coord_t dy = (lv_coord_t)(-ay * sensitivity); // invert vertical if needed
            lv_obj_align_to(g_img3, g_img2, LV_ALIGN_CENTER, dx, dy);
        }

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
    Serial.begin(115200);

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
