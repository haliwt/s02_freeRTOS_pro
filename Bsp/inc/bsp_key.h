#ifndef __BSP_KEY_H_
#define __BSP_KEY_H_
#include "main.h"

#define KEY_POWER_VALUE()    HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port, KEY_POWER_Pin)


typedef enum{

   power_off,
   power_on,
   mode_set_timer,
   mode_confirm,
   mode_set_temp,
   set_timer_confirm,
   counter_up,
   counter_down,
   key_sound
   
}power_state;

 typedef struct _key_fun{
   
   uint8_t  key_power;
   uint8_t  key_mode;
   uint8_t  key_sound_flag;
   uint8_t  key_select;

   uint8_t  set_temp_value;
   uint8_t   key_mode_times;
   
   uint8_t  gTimer_key_temp_timing;
   uint8_t  gTimer_set_timer_blink;
   uint8_t  gTimer_power_off;
	

}key_fun_t;


 extern key_fun_t gkey_t;






#endif 

