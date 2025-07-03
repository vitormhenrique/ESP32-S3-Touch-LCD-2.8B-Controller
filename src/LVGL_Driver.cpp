/*****************************************************************************
  | File        :   LVGL_Driver.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include "LVGL_Driver.h"
#include <esp_heap_caps.h>

// LVGL 9 API - using lv_display_t and lv_indev_t
lv_display_t * display = NULL;
lv_indev_t * indev = NULL;

void* buf1 = NULL;
void* buf2 = NULL;

/* 1.  One full‑screen buffer (RGB565) for rotation */
static uint8_t* rot_buf = NULL;

void allocate_rotation_buffer() {
    rot_buf = (uint8_t*)heap_caps_malloc(LVGL_WIDTH * LVGL_HEIGHT * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (rot_buf == NULL) {
        Serial.println("Failed to allocate PSRAM for rotation buffer!");
        while(1); // or handle error gracefully
    }
}


/* Serial debugging */
void Lvgl_print(const char * buf)
{
    // Serial.printf(buf);
    // Serial.flush();
}

/*  Display flushing 
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
void Lvgl_Display_LCD(lv_display_t *disp,
                      const lv_area_t *area,
                      uint8_t *px_map)
{
    lv_display_rotation_t rot = lv_display_get_rotation(disp);

    lv_area_t phys = *area;

    if(rot != LV_DISPLAY_ROTATION_0) {
        lv_display_rotate_area(disp, &phys);
        
        lv_color_format_t cf = lv_display_get_color_format(disp);
        uint32_t src_stride = lv_draw_buf_width_to_stride(lv_area_get_width(area), cf);
        uint32_t dest_stride = lv_draw_buf_width_to_stride(lv_area_get_width(&phys), cf);

        lv_draw_sw_rotate(px_map, rot_buf,
                          lv_area_get_width(area),
                          lv_area_get_height(area),
                          src_stride, dest_stride,
                          rot, cf);

        px_map = rot_buf;
        area = &phys;
    }

    LCD_addWindow(area->x1, area->y1, area->x2, area->y2, px_map);
    lv_display_flush_ready(disp);
}
/*Read the touchpad*/
void Lvgl_Touchpad_Read( lv_indev_t * indev, lv_indev_data_t * data )
{
  uint16_t touchpad_x[GT911_LCD_TOUCH_MAX_POINTS] = {0};
  uint16_t touchpad_y[GT911_LCD_TOUCH_MAX_POINTS] = {0};
  uint16_t strength[GT911_LCD_TOUCH_MAX_POINTS]   = {0};
  uint8_t touchpad_cnt = 0;
  
  Touch_Read_Data();
  uint8_t touchpad_pressed = Touch_Get_XY(touchpad_x, touchpad_y, strength, &touchpad_cnt, GT911_LCD_TOUCH_MAX_POINTS);
  
  if (touchpad_pressed && touchpad_cnt > 0) {
    // Get the display rotation
    lv_display_rotation_t rotation = lv_display_get_rotation(display);
    
    // Raw touch coordinates from sensor
    uint16_t raw_x = touchpad_x[0];
    uint16_t raw_y = touchpad_y[0];
    
    // Transform coordinates based on rotation to LVGL logical coordinates
    uint16_t transformed_x, transformed_y;
    
    switch(rotation) {
      case LV_DISPLAY_ROTATION_0:
        // No rotation - direct mapping
        transformed_x = raw_x;
        transformed_y = raw_y;
        break;
        
      case LV_DISPLAY_ROTATION_90:
        transformed_x = raw_y;
        transformed_y = ESP_PANEL_LCD_WIDTH- raw_x;
        break;
        
      case LV_DISPLAY_ROTATION_180:
        // 180 degrees: (x,y) -> (ESP_PANEL_LCD_WIDTH-x, ESP_PANEL_LCD_HEIGHT-y)
        // todo: UNTESTED
        transformed_x = ESP_PANEL_LCD_WIDTH - raw_x;
        transformed_y = ESP_PANEL_LCD_HEIGHT - raw_y;
        break;
        
      case LV_DISPLAY_ROTATION_270:
        // 270 degrees clockwise: (x,y) -> (y, ESP_PANEL_LCD_HEIGHT-x)
        // todo: UNTESTED
        transformed_x = raw_y;
        transformed_y = ESP_PANEL_LCD_HEIGHT - raw_x;
        break;
        
      default:
        // Fallback to no rotation
        transformed_x = raw_x;
        transformed_y = raw_y;
        break;
    }
    
    // Get current logical display dimensions for bounds checking
    int32_t logical_width = lv_display_get_horizontal_resolution(display);
    int32_t logical_height = lv_display_get_vertical_resolution(display);
    
    // Ensure coordinates are within current logical bounds
    if (transformed_x >= logical_width) transformed_x = logical_width - 1;
    if (transformed_y >= logical_height) transformed_y = logical_height - 1;
    
    data->point.x = transformed_x;
    data->point.y = transformed_y;
    data->state = LV_INDEV_STATE_PRESSED;
    
    printf("LVGL Touch: Raw(%u,%u) -> Logical(%u,%u) Rotation=%d LogicalRes=%ldx%ld\r\n", 
           raw_x, raw_y, transformed_x, transformed_y, rotation, logical_width, logical_height);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}
void Lvgl_Init(void)
{
  lv_init();
  
  // Allocate buffers in SPIRAM
  buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN, MALLOC_CAP_SPIRAM);
  buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN, MALLOC_CAP_SPIRAM);
  
  // Create display using new LVGL 9 API
  display = lv_display_create(LVGL_WIDTH, LVGL_HEIGHT);

  lv_disp_set_rotation(display, LV_DISPLAY_ROTATION_90); // Set rotation if needed
  
  // Set the flush callback
  lv_display_set_flush_cb(display, Lvgl_Display_LCD);
  
  // Set the buffers - note: size is now in bytes, not pixels
  lv_display_set_buffers(display, buf1, buf2, LVGL_BUF_LEN, LV_DISPLAY_RENDER_MODE_PARTIAL);
  
  // Set user data if needed
  lv_display_set_user_data(display, panel_handle);

  // Create input device using new LVGL 9 API
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, Lvgl_Touchpad_Read);

  // Create simple label - using updated API
  lv_obj_t *label = lv_label_create( lv_screen_active() );
  lv_label_set_text( label, "Hello Arduino and LVGL 9!");
  lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

  // Setup timer for LVGL tick
  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);
}
void Lvgl_Loop(void)
{
  lv_timer_handler(); /* let the GUI do its work */
  // delay( 5 );
}
  // delay( 5 );

