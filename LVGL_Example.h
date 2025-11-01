#pragma once

#include "LVGL_Driver.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "Wireless.h"
#include "BAT_Driver.h"
#include "Display_ST7701.h"  
#include "lvgl.h"

// LVGL image declarations (via LV_IMG_DECLARE in .cpp)
//axis change wall mount
#include "LVGL_Example.h"  

#define EXAMPLE1_LVGL_TICK_PERIOD_MS  1000

/*-----------------------------------
 * Backlight adjustment functions
 *----------------------------------*/
void Backlight_adjustment_event_cb(lv_event_t * e);
void LVGL_Backlight_adjustment(uint8_t Backlight);

/*-----------------------------------
 * LVGL Example 1 (3-image implementation)
 *----------------------------------*/
void Lvgl_Example1(void);

/*-----------------------------------
 * Overlay movement API
 *----------------------------------*/
void Lvgl_MoveOverlay(int16_t dx, int16_t dy);  // moves overlay relative to bg2 center

/*-----------------------------------
 * Background switch API
 *----------------------------------*/
void Lvgl_SwitchBackground(void);

/*-----------------------------------
 * Global LVGL image objects
 *----------------------------------*/
extern lv_obj_t * g_img1;  // bg1
extern lv_obj_t * g_img2;  // bg2
extern lv_obj_t * g_img3;  // overlay
