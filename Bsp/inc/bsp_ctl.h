#ifndef __BSP_CTL_H
#define __BSP_CTL_H
#include "main.h"


typedef enum{

   close,
   open
  
}hard_state;


typedef enum{

   good,
   error

}hard_error_state;

typedef struct _bsp_ctl{

    uint8_t ptc_flag;
	uint8_t plasma_flag;
	uint8_t ultrasonic_flag;
    uint8_t  ai_flag;
    uint8_t  wifi_flag;
    uint8_t gSet_temperature_value ;
    uint8_t run_process_step;

	uint8_t ptc_warning;
	uint8_t fan_warning;

    //wifi fun
    uint8_t disp_works_hours;
    uint8_t disp_works_minutes;
    uint8_t beijing_time_flag;
    uint8_t gTimer_pro_mode_key_be_select;

    uint8_t gTimer_prcoess_iwdg;
	uint8_t gTimer_process_display;
    uint8_t gTimer_wifi_blink;
    uint8_t gTimer_pro_action_publis;
    uint8_t gTimer_pro_pub_set_timer;
    uint8_t gTimer_wifi_ms_blink ;
	

}bsp_ctl;

extern bsp_ctl gctl_t;

extern uint8_t (*ptc_state)(void); //adjust of ptc is open or close
extern uint8_t (*plasma_state)(void); //adjust of plasma is open or close
extern uint8_t (*ultrasonic_state)(void); //adjust of ultrasoic is open or close

extern uint8_t (*ai_mode_state)(void);

extern uint8_t (*power_on_state)(void);



extern uint8_t (*ptc_error_state)(void);
extern uint8_t (*fan_error_state)(void);


void bsp_ctl_init(void);


void Ptc_State_Handler( uint8_t (*ptc_handler)(void));
void  Plasma_State_Handler(uint8_t(*plasma_handler)(void));
void  Ultrasonic_state_Handler(uint8_t(*ultrasonic_handler)(void));
void  Fan_state_Handler(uint8_t(*fan_handler)(void));
void  Ai_Mode_Handler(uint8_t(*ai_handler)(void));
void  Power_On_Handler(uint8_t(*poweron_handler)(void));


void  Ptc_error_state_Handler(uint8_t(*error_handler)(void));
void  Fan_error_state_Handler(uint8_t(*fan_error_handler)(void));




uint8_t Ptc_Default_Handler(void);
uint8_t Plasma_Default_Handler(void);
uint8_t Ultrasonic_Default_Handler(void);
uint8_t Fan_Default_Handler(void);

uint8_t Ptc_Error_Default_Handler(void);

uint8_t Fan_Error_Default_Handler(void);













#endif 
