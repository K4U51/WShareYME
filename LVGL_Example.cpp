#include "LVGL_Example.h"

/*-----------------------------------
 * Global LVGL image objects
 *----------------------------------*/
lv_obj_t * g_img1 = NULL;
lv_obj_t * g_img2 = NULL;
lv_obj_t * g_img3 = NULL;

/*-----------------------------------
 * Example: 3-image screen
 *----------------------------------*/
void Lvgl_Example1(void)
{
    // Create new screen
    lv_obj_t * scr = lv_obj_create(NULL);

    // -------------------------
    // Background 1 (visible)
    // -------------------------
    LV_IMG_DECLARE(bg1);
    g_img1 = lv_img_create(scr);
    lv_img_set_src(g_img1, &bg1);
    lv_obj_center(g_img1);

    // -------------------------
    // Background 2 (hidden initially)
    // -------------------------
    LV_IMG_DECLARE(bg2);
    g_img2 = lv_img_create(scr);
    lv_img_set_src(g_img2, &bg2);
    lv_obj_center(g_img2);
    lv_obj_add_flag(g_img2, LV_OBJ_FLAG_HIDDEN);

    // -------------------------
    // Overlay Image (centered)
    // -------------------------
    LV_IMG_DECLARE(overlay);
    g_img3 = lv_img_create(scr);
    lv_img_set_src(g_img3, &overlay);
    lv_obj_center(g_img3); // Start exactly in center  lv_obj_align(g_img3, LV_ALIGN_CENTER, 100, 100);

    // Load the screen
    lv_scr_load(scr);
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
 * Switch background image (with fade)
 *----------------------------------*/
void Lvgl_SwitchBackground(void)
{
    if (!g_img1 || !g_img2) return;

    // Hide bg1 and show bg2
    lv_obj_add_flag(g_img1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_img2, LV_OBJ_FLAG_HIDDEN);

    // Optional fade-in animation
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, g_img2);
    lv_anim_set_values(&a, 0, 255);
    lv_anim_set_time(&a, 800); // 0.8 second fade
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_start(&a);
}
