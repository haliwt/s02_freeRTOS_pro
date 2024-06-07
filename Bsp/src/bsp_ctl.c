#include "bsp_ctl.h"
#include "bsp.h"

bsp_ctl gctl_t;

uint8_t (*power_on_state)(void);


uint8_t (*ptc_state)(void); //adjust of ptc is open or close
uint8_t (*plasma_state)(void); //adjust of plasma is open or close
uint8_t (*ultrasonic_state)(void); //adjust of ultrasoic is open or close

uint8_t (*ai_mode_state)(void);



uint8_t (*ptc_error_state)(void);
uint8_t (*fan_error_state)(void);


static uint8_t ai_mode_default(void);

//uint8_t (*wifi_link_net_state)(void);
//static uint8_t wifi_link_net_default(void);

static uint8_t power_on_default(void);



void bsp_ctl_init(void)
{

   gctl_t.fan_warning =0;
   gctl_t.ptc_warning = 0;
   gctl_t.ptc_flag=1;
   gctl_t.plasma_flag =1;
   gctl_t.ultrasonic_flag =1;
   gctl_t.ai_flag = 1;  //default is "1" AI MODE
   gkey_t.key_add_dec_mode = set_temp_value_item;
   wifi_t.set_wind_speed_value=0;

   Buzzer_Sound_Init();
   
   Wifi_Init();
   Ptc_State_Handler(Ptc_Default_Handler);
   Plasma_State_Handler(Plasma_Default_Handler);
   Ultrasonic_state_Handler(Ultrasonic_Default_Handler);
   Ptc_error_state_Handler(Ptc_Error_Default_Handler);
   Fan_error_state_Handler(Fan_Error_Default_Handler);



    Ai_Mode_Handler(ai_mode_default);

    Power_On_Handler(power_on_default);

  
}

void main_fun_init(void)
{
   gctl_t.fan_warning =0;
   gctl_t.ptc_warning = 0;
   gctl_t.ptc_flag=1;
   gctl_t.plasma_flag =1;
   gctl_t.ultrasonic_flag =1;
   gctl_t.ai_flag = 1;
   gkey_t.key_add_dec_mode = set_temp_value_item;
   wifi_t.set_wind_speed_value=0;



}


/*****************************************************************************
 * 
 * Function Name: uint8_t Ptc_State_Handler( uint8_t (*ptc_dat)(uint8_t idat))
 * Function:
 * Input Ref:
 * Return Ref:
 * 
*****************************************************************************/
void Ptc_State_Handler( uint8_t (*ptc_handler)(void))
{
   	ptc_state = ptc_handler ;
    
}

uint8_t Ptc_Default_Handler(void)
{
    if(gctl_t.ptc_flag == 1){
        return 1;
	}
	else{
        return 0;

	}
	

}
/*****************************************************************************
 * 
 * Function Name: uint8_t Plasma_State_Handler(uint8_t(*plasma_handler)(uint8_t ipa))
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/
void Plasma_State_Handler(uint8_t(*plasma_handler)(void))
{
	 plasma_state = plasma_handler;

}

uint8_t Plasma_Default_Handler(void)
{
  
	if(gctl_t.plasma_flag == 1){
        return 1;
	}
	else{
        return 0;

	}
	

}

/*****************************************************************************
 * 
 * Function Name: void Ptc_Default_Handler(void)
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/
void  Ultrasonic_state_Handler(uint8_t(*ultrasonic_handler)(void))
{
		ultrasonic_state = ultrasonic_handler;
}

uint8_t Ultrasonic_Default_Handler(void)
{
	
	if(gctl_t.ultrasonic_flag == 1){
        return 1;
	}
	else{
        return 0;

	}
	
}
/*****************************************************************************
 * 
 * Function Name: uint8_t Error_state_Handeler(uint8_t(*error_handler)(uint8_t idat))
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/
void  Ptc_error_state_Handler(uint8_t(*ptc_error_handler)(void))
{
      ptc_error_state = ptc_error_handler;
}

uint8_t Ptc_Error_Default_Handler(void)
{
    
	if(gctl_t.ptc_warning == 1){
        return 1;
	}
	else{
		return 0;

	}
    


}
/*****************************************************************************
 * 
 * Function Name: uint8_t Fan_error_state_Handler(uint8_t(*fan_error_handler)(uint8_t fan_data))
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/
void Fan_error_state_Handler(uint8_t(*fan_error_handler)(void))
{
      fan_error_state = fan_error_handler;
}

uint8_t Fan_Error_Default_Handler(void)
{

	if(gctl_t.fan_warning == 1){

	    return 1;
	}
	else{
	   return 0;
	}
    
}

/*****************************************************************************
 * 
 * Function Name: void  Ai_Mode_Handler(uint8_t(*ai_handler)(void))
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/
static uint8_t ai_mode_default(void)
{
     if(gctl_t.ai_flag == 1)return 1;
     else return 0;

}




void  Ai_Mode_Handler(uint8_t(*ai_handler)(void))
{

      ai_mode_state = ai_handler;

}

/*****************************************************************************
 * 
 * Function Name:  Wifi_Link_Net_Handler(uint8_t(*wifi_handler)(void))
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/
static uint8_t power_on_default(void)
{

    if(gkey_t.key_power ==power_on)return 1;
    else return 0;
   


}

void  Power_On_Handler(uint8_t(*power_handler)(void))

{

    power_on_state = power_handler;

}

/*****************************************************************************
 * 
 * Function Name:  Wifi_Link_Net_Handler(uint8_t(*wifi_handler)(void))
 * Function:
 * Input Ref: idata is ptc is open "1" or "0"  close 
 * Return Ref: close or open 
 * 
*****************************************************************************/









