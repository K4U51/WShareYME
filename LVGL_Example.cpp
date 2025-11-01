#include "LVGL_Example.h"

/*-----------------------------------
 * Global LVGL image objects
 *----------------------------------*/
lv_obj_t * g_img1;
lv_obj_t * g_img2;
lv_obj_t * g_img3;

/*-----------------------------------
 * Commented old style / font objects
 *----------------------------------*/
// static disp_size_t disp_size;
// static lv_obj_t * tv;
// lv_style_t style_text_muted;
// lv_style_t style_title;
// static lv_style_t style_icon;
// static lv_style_t style_bullet;
// static const lv_font_t * font_large;
// static const lv_font_t * font_normal;
// static lv_timer_t * auto_step_timer;
// static lv_timer_t * meter2_timer;

/*-----------------------------------
 *  Example 3-image screen
 *----------------------------------*/
void Lvgl_Example1(void)
{
    // -------------------------
    // Old style / font init (commented)
    // -------------------------
    /*
    if(LV_HOR_RES <= 320) disp_size = DISP_SMALL;
    else if(LV_HOR_RES < 720) disp_size = DISP_MEDIUM;
    else disp_size = DISP_LARGE;

    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    lv_style_init(&style_text_muted);
    lv_style_set_text_opa(&style_text_muted, LV_OPA_90);

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);

    lv_style_init(&style_icon);
    lv_style_set_text_color(&style_icon, lv_theme_get_color_primary(NULL));
    lv_style_set_text_font(&style_icon, font_large);

    lv_style_init(&style_bullet);
    lv_style_set_border_width(&style_bullet, 0);
    lv_style_set_radius(&style_bullet, LV_RADIUS_CIRCLE);
    */

    // -------------------------
    // Create new screen
    // -------------------------
    lv_obj_t * scr = lv_obj_create(NULL);

    // -------------------------
    // Background 1 (visible)
    // -------------------------
    g_img1 = lv_img_create(scr);
    LV_IMG_DECLARE(ui_img_bg1_png);  // Your first image asset
    lv_img_set_src(g_img1, &ui_img_bg1_png);
    lv_obj_center(g_img1);

    // -------------------------
    // Background 2 (hidden initially)
    // -------------------------
    g_img2 = lv_img_create(scr);
    LV_IMG_DECLARE(ui_img_bg2_png);  // Your second image asset
    lv_img_set_src(g_img2, &ui_img_bg2_png);
    lv_obj_center(g_img2);
    lv_obj_add_flag(g_img2, LV_OBJ_FLAG_HIDDEN);

    // -------------------------
    // Overlay Image 3
    // -------------------------
    g_img3 = lv_img_create(scr);
    LV_IMG_DECLARE(ui_img_overlay_png); // Overlay image asset
    lv_img_set_src(g_img3, &ui_img_overlay_png);
    lv_obj_align(g_img3, LV_ALIGN_CENTER, 0, 0); // Start at center

    // Load the screen
    lv_scr_load(scr);

    // -------------------------
    // Optional: automatic background switch (commented)
    // -------------------------
    /*
    lv_timer_t * t = lv_timer_create_basic();
    lv_timer_set_cb(t, (lv_timer_cb_t)Lvgl_SwitchBackground);
    lv_timer_set_period(t, 3000);
    */
}

/*-----------------------------------
 * Move overlay dynamically
 *----------------------------------*/
void Lvgl_MoveOverlay(int16_t x, int16_t y)
{
    if (!g_img3) return;
    lv_obj_set_pos(g_img3, x, y);
}

/*-----------------------------------
 * Switch background image
 *----------------------------------*/
void Lvgl_SwitchBackground(void)
{
    if (!g_img1 || !g_img2) return;
    lv_obj_add_flag(g_img1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_img2, LV_OBJ_FLAG_HIDDEN);
}
