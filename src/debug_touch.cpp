#include "LVGL_Driver.h"
#include "ui/ui.h"

// Debug function to be called after UI init
void debug_touch_areas() {
    printf("\n=== TOUCH DEBUG INFO ===\n");
    
    // Display info
    lv_display_t* disp = lv_display_get_default();
    if (disp) {
        lv_display_rotation_t rotation = lv_display_get_rotation(disp);
        int32_t width = lv_display_get_horizontal_resolution(disp);
        int32_t height = lv_display_get_vertical_resolution(disp);
        printf("Display: %ldx%ld, Rotation: %d\n", width, height, rotation);
    }
    
    // Screen info
    lv_obj_t* active_screen = lv_screen_active();
    if (active_screen) {
        lv_area_t screen_area;
        lv_obj_get_coords(active_screen, &screen_area);
        printf("Active Screen area: x1:%d y1:%d x2:%d y2:%d\n", 
               screen_area.x1, screen_area.y1, screen_area.x2, screen_area.y2);
    }
    
    // Button info (if Screen1 is active)
    extern lv_obj_t *ui_Button1;
    if (ui_Button1) {
        lv_area_t btn_area;
        lv_obj_get_coords(ui_Button1, &btn_area);
        printf("Button1 area: x1:%d y1:%d x2:%d y2:%d (w:%d h:%d)\n", 
               btn_area.x1, btn_area.y1, btn_area.x2, btn_area.y2,
               lv_area_get_width(&btn_area), lv_area_get_height(&btn_area));
        
        // Check button flags
        bool clickable = lv_obj_has_flag(ui_Button1, LV_OBJ_FLAG_CLICKABLE);
        bool hidden = lv_obj_has_flag(ui_Button1, LV_OBJ_FLAG_HIDDEN);
        bool scrollable = lv_obj_has_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);
        printf("Button1 flags: clickable:%d hidden:%d scrollable:%d\n", 
               clickable, hidden, scrollable);
        
        // Check if button has parent
        lv_obj_t* parent = lv_obj_get_parent(ui_Button1);
        printf("Button1 parent: %p\n", parent);
    }
    
    printf("========================\n\n");
}

// Function to test if a point hits the button
void test_touch_point(int16_t x, int16_t y) {
    extern lv_obj_t *ui_Button1;
    if (!ui_Button1) {
        printf("Button1 not available for hit test\n");
        return;
    }
    
    lv_area_t btn_area;
    lv_obj_get_coords(ui_Button1, &btn_area);
    
    bool hit = (x >= btn_area.x1 && x <= btn_area.x2 && 
                y >= btn_area.y1 && y <= btn_area.y2);
    
    printf("Touch test at (%d,%d): %s button area [%d,%d,%d,%d]\n", 
           x, y, hit ? "HIT" : "MISS", 
           btn_area.x1, btn_area.y1, btn_area.x2, btn_area.y2);
}
