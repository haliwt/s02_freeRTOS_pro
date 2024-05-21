#ifndef __BSP_KEY_H_
#define __BSP_KEY_H_
#include "main.h"

#define KEY_POWER_VALUE()    HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port, KEY_POWER_Pin)

#define KEY_MODE_VALUE()     HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port, KEY_MODE_Pin)

#define KEY_DEC_VALUE()    HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin)


typedef enum{

   power_off,
   power_on,

}power_state;


typedef enum mode_key_t{
    
   disp_works_timing,
   disp_timer_timing,
   
   mode_set_timer,
   mode_confirm,
   mode_set_temp,
   set_timer_confirm,
   counter_up,
   counter_down,
   key_sound
   
}mode_state;

 typedef struct _key_fun{
   
   uint8_t  key_power;
   uint8_t  key_mode;
   uint8_t  key_sound_flag;
   uint8_t  key_select;

   uint8_t  set_temp_value;
   uint8_t   key_mode_times;
   uint8_t   gTimer_disp_switch_disp_mode ;
   
   uint8_t  gTimer_key_temp_timing;
   uint8_t  gTimer_disp_set_timer_blink;
   uint8_t  gTimer_power_off;
   uint8_t  gTimer_disp_set_timer;
	

}key_fun_t;


 extern key_fun_t gkey_t;


void Dec_Key_Fun(uint8_t cmd);

void Add_Key_Fun(uint8_t cmd);



#endif 

