#include <Arduino.h>
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "LVGL_Driver.h"
#include "BAT_Driver.h"
#include "ui/ui.h"

void Driver_Init()
{
  Flash_test();
  BAT_Init();
  I2C_Init();
  PCF85063_Init();
  QMI8658_Init();    
  TCA9554PWR_Init(0x00);
  Set_EXIO(EXIO_PIN8,Low);
  Backlight_Init();
}
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

static void global_touch_event(lv_event_t * e) {
    lv_indev_t * indev = lv_indev_get_act();
    lv_point_t pos;
    lv_indev_get_point(indev, &pos);
    printf("[Global Touch] x: %d, y: %d\n", pos.x, pos.y);
}

void setup()
{
  delay(1000);
  allocate_rotation_buffer();
  Driver_Init();
  LCD_Init();                                     // If you later reinitialize the LCD, you must initialize the SD card again !!!!!!!!!!
  SD_Init(); // It must be initialized after the LCD, and if the LCD is reinitialized later, the SD also needs to be reinitialized
  Lvgl_Init();

  ui_init();   
  
  // Debug touch areas after UI is fully initialized
  delay(100); // Give UI time to fully initialize
  // debug_touch_areas();
  
  // lv_obj_add_event_cb(lv_scr_act(), global_touch_event, LV_EVENT_CLICKED, NULL); //

  
  xTaskCreatePinnedToCore(
    Driver_Loop,     
    "Other Driver task",   
    4096,                
    NULL,                 
    3,                    
    NULL,                
    0                    
  );
}
void loop()
{
  Lvgl_Loop();
  delay(5);
}
