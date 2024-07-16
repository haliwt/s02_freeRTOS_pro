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







void freeRTOS_Handler(void);


void smartphone_turn_on_handler(void);



#endif

