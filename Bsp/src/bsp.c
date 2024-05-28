#include "bsp.h"


BSP_process_t gpro_t;
static uint8_t Works_Time_Out(void);
static void Mainboard_Action_Fun(void);
static void Mainboard_Fun_Stop(void);




static void Display_LCD_Works_Timing(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- 喂狗 8s input reset */
//    if(gctl_t.gTimer_prcoess_iwdg > 5){
//		gctl_t.gTimer_prcoess_iwdg =0;
//    	iwdg_feed();
//
//    }

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();

 
  LCD_Timer_Colon_Flicker();
	
	//MODH_Poll();
}
/*
*********************************************************************************************************
*	函 数 名: void mainboard_process_handler(void)
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*	函 数 名: static uint8_t Works_Time_Out(void)
*	功能说明: 主板工作2小时，停止工作10分钟
*			 
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t fan_continue_flag;
static uint8_t Works_Time_Out(void)
{
	if(gpro_t.gTimer_run_time_out < 11){
		
		Mainboard_Fun_Stop();
		 
    }

	if(gpro_t.gTimer_run_one_mintue < 60 && ( fan_continue_flag ==0)){

		Fan_Run();

	}

	if(gpro_t.gTimer_run_one_mintue > 60){

	     fan_continue_flag=1;

         Fan_Stop();
	 }

	if(gpro_t.gTimer_run_time_out > 10){ //10 minutes
		gpro_t.gTimer_run_time_out=0;
		gpro_t.gTimer_run_total=0;

		Mainboard_Action_Fun();
		

		return 1;


	}
	else{
      
	  return 0;

	}

}


/*
*********************************************************************************************************
*	函 数 名: static void Mainboard_Action_Fun(void)
*	功能说明: 主板工作：功能动作输出
*			 
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Mainboard_Action_Fun(void)
{
    Ptc_On();
	Ultrasonic_Pwm_Output();
	Fan_Run();
	Plasma_On();
	
	
}

/*
*********************************************************************************************************
*
*	函 数 名: static void Mainboard_Action_Fun(void)
*	功能说明: 主板工作：功能动作输出			 
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
static void Mainboard_Fun_Stop(void)
{
   Ptc_Off();

   Ultrasonic_Pwm_Stop();
   Fan_Stop();
   Plasma_Off();


}


/*
*********************************************************************************************************
*
*	函 数 名: void Set_Timer_Timing_Lcd_Blink(void )
*	功能说明: 设置的定时时间闪烁
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
#if 0
void Set_Timer_Timing_Lcd_Blink(uint8_t hours,uint8_t minutes)
{
    if(gpro_t.gTimer_disp_set_timer_blink < 1){//3* 100ms

	  glcd_t.number5_low =  0x0A ;
      glcd_t.number5_high =  0x0A ;

      glcd_t.number6_low  =  0x0A; //
      glcd_t.number6_high =  0x0A; //
      
       //dispaly minutes 
      glcd_t.number7_low =   0x0A;
      glcd_t.number7_high =   0x0A;

      glcd_t.number8_low =   0x0A;
      glcd_t.number8_high =   0x0A;

    


    }
	else if(gpro_t.gTimer_disp_set_timer_blink > 0 && gpro_t.gTimer_disp_set_timer_blink < 2){
	


      glcd_t.number5_low =  gpro_t.set_timer_timing_hours / 10 ;
      glcd_t.number5_high =  gpro_t.set_timer_timing_hours / 10 ;

      glcd_t.number6_low  = gpro_t.set_timer_timing_hours% 10; //
      glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10; //
      
       //dispaly minutes 
      glcd_t.number7_low =  0;
      glcd_t.number7_high =  0;

      glcd_t.number8_low = 0;
      glcd_t.number8_high =   0;

       
	}
	else if(gpro_t.gTimer_disp_set_timer_blink > 1){
	  gpro_t.gTimer_disp_set_timer_blink =0;

    }

  	 LCD_Disp_Timer_Timing();

}

#endif 



/*
*********************************************************************************************************
*
*	函 数 名: void Process_Dynamical_Action(void)
*	功能说明: 主板工作：功能动作输出			 
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
void Process_Dynamical_Action(void)
{

   static uint8_t the_send_open_ptc,to_tenced_data,ptc_int=0xff,ptc_int_1=0xff;
   static uint8_t ptc_int_off =0xff,ptc_int_off_1=0xff,to_tenced_off_data;
   if(gpro_t.set_temperature_value_success == 1){
       if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value ){//if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

              if(the_send_open_ptc==1){
                 the_send_open_ptc ++;
                 to_tenced_off_data++;
                gctl_t.ptc_flag =1;
                Ptc_On();
                Disp_Dry_Icon();
                if(wifi_link_net_state()==1 &&  ptc_int_1 != to_tenced_data){
                   ptc_int_1 = to_tenced_data;
                  MqttData_Publish_SetPtc(1);
                  HAL_Delay(350);
                 }

               }
               else if(the_send_open_ptc==2){
                 if((gctl_t.gSet_temperature_value -3) > gctl_t.dht11_temp_value ){

                  to_tenced_off_data++;
                   gctl_t.ptc_flag =1;
                   Ptc_On();
                   Disp_Dry_Icon();
                    if(wifi_link_net_state()==1 && ptc_int !=to_tenced_data ){
                      ptc_int =to_tenced_data  ; 
                      MqttData_Publish_SetPtc(1);
                      HAL_Delay(350);
                   }
                  }

               }
               else{
                  gctl_t.ptc_flag =1;
                  Ptc_On();
                   to_tenced_off_data++;
                  Disp_Dry_Icon();
                  

               }
               

       }
       else if(gctl_t.gSet_temperature_value <  gctl_t.dht11_temp_value){

              if(the_send_open_ptc==0){
                   the_send_open_ptc++;
                  gctl_t.ptc_flag =0;
                  Ptc_Off(); 
                  Disp_Dry_Icon();
                  to_tenced_data++;
                  if(wifi_link_net_state()==1 && ptc_int_off_1 !=to_tenced_off_data ){
                      ptc_int_off_1 =to_tenced_off_data ; 
                      MqttData_Publish_SetPtc(0);
                      HAL_Delay(350);
                   }

              }
              else{
                  gctl_t.ptc_flag =0;
                  Ptc_Off(); 
                  Disp_Dry_Icon();
                   if(wifi_link_net_state()==1 && ptc_int_off !=to_tenced_off_data ){
                      ptc_int_off =to_tenced_off_data;
                      MqttData_Publish_SetPtc(0);
                      HAL_Delay(350);
                   }
                  to_tenced_data++;


              }

       }

   
    }
    else{
        
        if(ptc_state() ==1){

         if(gctl_t.dht11_temp_value > 39){
             gctl_t.ptc_flag =0;
             Ptc_Off();
             Disp_Dry_Icon();
         }
         else{

    	   Ptc_On();
           Disp_Dry_Icon();
         }

    	}
    	else{
          Ptc_Off();
          Disp_Dry_Icon();

    	}
    }

	if(plasma_state() ==1){
		
       Plasma_On();

    }
	else{

	   Plasma_Off();

	}

    if(ultrasonic_state() ==1){

         Ultrasonic_Pwm_Output();


    }
    else{

    Ultrasonic_Pwm_Stop();


    }

  
    switch(wifi_t.set_wind_speed_value){
    
         case 0: //full speed
    
    
           Fan_Run();
    
         break;
    
         case 1 : //middle speed
          Fan_Run_Middle();
    
         break;
    
         case 2: //lower speed
          Fan_Run_Lower();
         break;
    
    
       }

}


