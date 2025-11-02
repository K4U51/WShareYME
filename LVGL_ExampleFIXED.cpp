//changed a axis and orientation
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
       // lv_obj_set_size(g_img2, 480, 480); possible fix for roundng box
    lv_obj_center(g_img2);
    lv_obj_add_flag(g_img2, LV_OBJ_FLAG_HIDDEN);

    // -------------------------
    // Overlay Image (centered on bg2)
    // -------------------------
    LV_IMG_DECLARE(overlay);
    g_img3 = lv_img_create(scr);
    lv_img_set_src(g_img3, &overlay);

    // Align overlay on bg2 (exact center)
    lv_obj_align_to(g_img3, g_img2, LV_ALIGN_CENTER, 0, 0);

    // Load the screen
    lv_scr_load(scr);
}

/*-----------------------------------
 * Move overlay dynamically relative to bg2
 *----------------------------------*/
void Lvgl_MoveOverlay(int16_t dx, int16_t dy)
{
    if (!g_img3 || !g_img2) return;

    // Move overlay relative to bg2 center
    lv_obj_align_to(g_img3, g_img2, LV_ALIGN_CENTER, dx, dy);
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
