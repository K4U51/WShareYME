#include "Wireless.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "BAT_Driver.h"

//-----------------------------
// Optional: overlay movement task
//-----------------------------
void Lvgl_UpdateOverlayFromGyro(void * parameter)
{
    while(1)
    {
        float ax, ay, az;
        QMI8658_Read(&ax, &ay, &az); // read gyro/accelerometer

        // Map acceleration to screen coordinates
        int16_t x = (int16_t)(ax * 50) + 120; // 240x240 display center
        int16_t y = (int16_t)(-ay * 50) + 120;

        Lvgl_MoveOverlay(x, y); // move overlay
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

//-----------------------------
// Driver Task
//-----------------------------
void Driver_Loop(void *parameter)
{
    while(1)
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
    
    xTaskCreatePinnedToCore(
        Driver_Loop,     
        "Other Driver task",   
        4096,                
        NULL,                 
        3,                    
        NULL,                
        0                    
    );

    // Create overlay movement task
    xTaskCreatePinnedToCore(
        Lvgl_UpdateOverlayFromGyro,
        "Overlay Task",
        2048,
        NULL,
        4,
        NULL,
        1
    );
}

//-----------------------------
// Setup
//-----------------------------
void setup()
{
    Wireless_Test2();
    Driver_Init();

    LCD_Init();    // Initialize SPI display
    SD_Init();     // Must be after LCD

    Lvgl_Init();   // Initialize LVGL library
    Lvgl_Example1(); // Load 3-image screen (simple version)
}

//-----------------------------
// Main loop
//-----------------------------
void loop()
{
    Lvgl_Loop();  // LVGL handler
    vTaskDelay(pdMS_TO_TICKS(5));
}
