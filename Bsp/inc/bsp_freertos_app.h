#ifndef __BSP_FREERTOS_APP_H
#define __BSP_FREERTOS_APP_H

#include "main.h"

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>

#include "FreeRTOS.h"
#include "task.h"

#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "interrupt_manager.h"



typedef struct Msg
{
   
    uint8_t modekey_detect;
    uint8_t set_timer_timing_success;
    
    uint8_t key_mode;
    uint8_t disp_timer_works_switch_flag;
	uint8_t usData[2];
	uint8_t ulData[2];

    uint16_t ucMessageID;
 
}MSG_T;

extern MSG_T   g_tMsg; /* ¶¨ÒåÒ»¸ö½á¹¹ÌåÓÃÓÚÏûÏ¢¶ÓÁÐ */



void freeRTOS_Handler(void);





#endif

