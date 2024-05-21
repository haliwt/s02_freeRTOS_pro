#ifndef  	__BSP_H_
#define 	__BSP_H_
#include "main.h"


#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>

#include "FreeRTOS.h"
#include "task.h"

#include "interrupt_manager.h"
#include "adc.h"

#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"



//
#include "bsp_adc.h"
#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_buzzer.h"
#include "bsp_key.h"
#include "bsp_sensor.h"
#include "bsp_ultrasonic.h"
#include "bsp_delay.h"
#include "bsp_ctl.h"
#include "bsp_fan.h"
#include "bsp_plasma.h"
#include "bsp_ptc.h"
#include "bsp_usart.h"
#include "bsp_freertos_app.h"

#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

typedef struct {

   uint8_t set_temp_confirm ;
   uint8_t gdht11_temperature;
   uint8_t gDht11_humidity;

   int8_t set_timer_timing_hours;
   int8_t set_timer_timing_minutes;


   uint8_t gTimer_run_adc;
   uint8_t gTimer_run_dht11;
   uint8_t gTimer_normal_display_lcd;
   uint8_t gTimer_run_one_mintue ;
   
   uint16_t gTimer_run_total;
   uint16_t gTimer_run_time_out;

   uint8_t gTimer_works_counter;
   uint8_t gTimer_display_works_hours;
   uint8_t gTimer_display_works_minutes;
   uint8_t gTimer_timer_Counter;
   uint8_t gTimer_disp_set_timer_blink ;


}BSP_process_t;

extern BSP_process_t gProcess_t;

void bsp_Idle(void);

void PowerOn_Init(void);


void mainboard_process_handler(void);


void PowerOff_freeFun(void);

void Run_Main_Handler(void);

void Display_Works_Timing(void);


void Display_Timer_Timing(int8_t hours,int8_t minutes);



void Set_Timer_Timing_Lcd_Blink(uint8_t hours,uint8_t minutes);


#endif 

