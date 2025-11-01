#pragma once

#include "LVGL_Driver.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "Wireless.h"
#include "BAT_Driver.h"
#include "Display_ST7701.h"  
#include "lvgl.h"
#include "Lvgl_Example.h"  // LV_IMG_DECLARE for your images

#define EXAMPLE1_LVGL_TICK_PERIOD_MS  1000

/*-----------------------------------
 * Backlight adjustment functions
 *----------------------------------*/
void Backlight_adjustment_event_cb(lv_event_t * e);
void LVGL_Backlight_adjustment(uint8_t Backlight);

/*-----------------------------------
 * LVGL Example 1 (OLD) - comment out until updated
 *----------------------------------*/
void Lvgl_Example1(void);  // OLD example, replace with new 3-image implementation later

/*-----------------------------------
 * Overlay movement API
 *----------------------------------*/
void Lvgl_MoveOverlay(int16_t x, int16_t y);

/*-----------------------------------
 * Background switch API
 *----------------------------------*/
void Lvgl_SwitchBackground(void);

/*-----------------------------------
 * Global LVGL image objects
 *----------------------------------*/
extern lv_obj_t * g_img1;
extern lv_obj_t * g_img2;
extern lv_obj_t * g_img3;
