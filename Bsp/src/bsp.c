#include "bsp.h"


BSP_process_t gProcess_t;
static uint8_t Works_Time_Out(void);
static void Mainboard_Action_Fun(void);
static void Mainboard_Fun_Stop(void);
static void Process_Dynamical_Action(void);
static void Display_Works_Timing(void);
static void Set_Timer_Timing_Lcd_Blink(void );

static void Display_Timer_Timing(int8_t hours,int8_t minutes);




uint8_t  fan_continue_flag;
uint8_t step_process;


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
void mainboard_process_handler(void)
{
    static uint8_t first_power_up, compare_temp_value, compare_temp_value_flag;
	static uint8_t  timer_timing_flag,set_timer_hours,set_timer_minutes;
	

//	if( gkey_t.key_sound_flag == key_sound){
//		gkey_t.key_sound_flag =0;
//		Buzzer_KeySound();
//
//    }

	switch(gkey_t.key_power){

      case power_off:

	    step_process=0;
		compare_temp_value_flag =0; //关闭ptc温度设置的值
	    timer_timing_flag = 0 ; //关闭定时标志位，取消定时功能
	    LED_Mode_Off();
	   
	   
        Ptc_Off();
		Ultrasonic_Pwm_Stop();
		Plasma_Off();
	
        if(first_power_up==1){
           if(gkey_t.gTimer_power_off < 61){
                Fan_Run();
				LCD_Display_Wind_Icon_Handler();

		   }
		   else{
              first_power_up =0;
			   Fan_Stop();
		       Backlight_Off();
		       Lcd_Display_Off();
			   
		   }

		}

		
        Breath_Led();

	  break;


	  case power_on: //1

        switch(step_process){


		  case 0:
              //led on 
              first_power_up=1;
//		     gProcess_t.gTimer_display_works_minutes=0;
//		      gProcess_t.gTimer_display_works_hours =0;
//		      
//		      LED_Mode_On();
//		      LED_Power_On();
//		      Backlight_On();
//		  
//			  Update_DHT11_Value();
//		      Lcd_Display_Detials();
//
//			  //fan on
//			  Mainboard_Action_Fun();
//		  
//		    step_process = 1;


		 // break;

		   case 1: // normal all content display
          
		    if(gProcess_t.gTimer_normal_display_lcd > 30){ //30*10 =300ms flash
			    gProcess_t.gTimer_normal_display_lcd=0;
				 
			    Lcd_Display_Detials();

				Process_Dynamical_Action();
				
 
            }

		     step_process=2;

		  case 2:   //run dht11 display 

		  if(gkey_t.key_select == mode_set_temp){ //set up temperature value
		    if(gkey_t.gTimer_key_temp_timing > 3){
                 gkey_t.key_select = 0;
				 compare_temp_value = gkey_t.set_temp_value;
				 //lcd digitall '3''4' blink
				 gProcess_t.set_temp_confirm = 1; //set_temperature vale blink is three times 
				 compare_temp_value_flag=1;
                 gkey_t.gTimer_key_temp_timing = 0;
			}

		  }
		  else if(gProcess_t.set_temp_confirm ==0){

			  if(gProcess_t.gTimer_run_dht11 > 36){
			  	gProcess_t.gTimer_run_dht11 =0;
		        Update_DHT11_Value();

	           }

			   if(compare_temp_value_flag ==1){
                   
				   if(compare_temp_value >= gProcess_t.gdht11_temperature){

					  gctl_t.ptc_flag  =1 ; //open ptc
				   }
				   else{
					  gctl_t.ptc_flag =0 ;  //close ptc 
				   }

			   }
			   else{
                    
					if(gProcess_t.gdht11_temperature >40){ 
						gctl_t.ptc_flag =0 ;  //close ptc 
					}
					else if(gProcess_t.gdht11_temperature <39){
                        gctl_t.ptc_flag =1 ; //open ptc 
                    }
                }

		  }
           step_process=3;
		  
          break;

		  case 3: //set timer timing value 
               if(gkey_t.key_mode == mode_set_timer){
			   	
                   Set_Timer_Timing_Lcd_Blink();

				   if(gkey_t.gTimer_set_timer_blink > 3){

                       gkey_t.key_mode=0;
					   gkey_t.key_mode_times=0;

				   }

			   }
			   else if(gkey_t.key_mode == mode_confirm){

			       if(gProcess_t.set_timer_timing_hours ==0 && gProcess_t.set_timer_timing_minutes==0 ){
			   	     timer_timing_flag = 0;
					 gkey_t.key_mode =0;

			       	}
				    else{
					  timer_timing_flag = 1;
					  gkey_t.key_mode =0;
					  set_timer_hours=gProcess_t.set_timer_timing_hours;
					  set_timer_minutes=gProcess_t.set_timer_timing_minutes;
					  gProcess_t.gTimer_timer_Counter=0;

					}
			   	

			   }
			   else if(timer_timing_flag == 1){
                    Display_Timer_Timing(set_timer_hours, set_timer_minutes);


			   }
			   else{
		          Display_Works_Timing();

			   }

		       step_process=4;
		  break;


		  case 4: //detected ptc temperature value and fan if fault

		  if(gProcess_t.gTimer_run_adc > 10 && gProcess_t.gTimer_run_adc < 12){ //3 minutes 120s
				
			  Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,5);
			  HAL_Delay(2);
					
          }

		  if(gProcess_t.gTimer_run_adc > 18){ //2 minute 180s
				gProcess_t.gTimer_run_adc=0;

				Get_Fan_Adc_Fun(ADC_CHANNEL_0,5);
				HAL_Delay(2);
	               

		 }

		  step_process=5;

		  break;

		  case 5: //check works times 
			  if(gProcess_t.gTimer_run_total > 119){ //120 minutes
			       gProcess_t.gTimer_run_total =0;
				   gProcess_t.gTimer_run_time_out=0;  //time out recoder start 10 minutes
				   gProcess_t.gTimer_run_one_mintue =0;
				   fan_continue_flag=0;
				   step_process=6;
			  
		         
			    }
			    else{
				  step_process=1;

              }

		  break;

		  case 6: //works have a rest ten minutes

		       if(Works_Time_Out()==1){

			        step_process=1;

			   }
			   else{

			      step_process=6;

			   }


		  break;

		  


	     }

        
	  break;

	 


    }

}


void PowerOn_Init(void)
{
  

    //gProcess_t.gTimer_display_works_minutes=0;
    ///gProcess_t.gTimer_display_works_hours =0;

    LED_Mode_On();
    LED_Power_On();
    Backlight_On();

   // Update_DHT11_Value();
   /// Lcd_Display_Detials();

    //fan on
   /// Mainboard_Action_Fun();

    

}

/*
*********************************************************************************************************
*	函 数 名: static uint8_t Works_Time_Out(void)
*	功能说明: 主板工作2小时，停止工作10分钟
*			 
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t Works_Time_Out(void)
{
	if(gProcess_t.gTimer_run_time_out < 11){
		
		Mainboard_Fun_Stop();
		 
    }

	if(gProcess_t.gTimer_run_one_mintue < 60 && ( fan_continue_flag ==0)){

		Fan_Run();

	}

	if(gProcess_t.gTimer_run_one_mintue > 60){

	     fan_continue_flag=1;

         Fan_Stop();
	 }

	if(gProcess_t.gTimer_run_time_out > 10){ //10 minutes
		gProcess_t.gTimer_run_time_out=0;
		gProcess_t.gTimer_run_total=0;

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
*	函 数 名: void Process_Dynamical_Action(void)
*	功能说明: 主板工作：功能动作输出			 
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
static void Process_Dynamical_Action(void)
{
	if(ptc_state() ==1){

	  Ptc_On();

	}
	else{
     Ptc_Off();

	}

	if(plasma_state() ==1){
		
       Plasma_On();

    }
	else{

	   Plasma_Off();

	}

}

/*
*********************************************************************************************************
*
*	函 数 名: void Display_Works_Timing(void)
*	功能说明: 显示设备工作的时间，时间最大值是 99个小时
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
static void Display_Works_Timing(void)
{

    if(gProcess_t.gTimer_works_counter > 59 ){

	  gProcess_t.gTimer_works_counter=0;

	  gProcess_t.gTimer_display_works_minutes++;

	if( gProcess_t.gTimer_display_works_minutes > 59){ //1 hours
		gProcess_t.gTimer_display_works_minutes=0;

	     gProcess_t.gTimer_display_works_hours++;


		glcd_t.number7_low = gProcess_t.gTimer_display_works_minutes / 10;
		glcd_t.number7_high = gProcess_t.gTimer_display_works_minutes / 10;


		glcd_t.number8_low = gProcess_t.gTimer_display_works_minutes  % 10;
		glcd_t.number8_high = gProcess_t.gTimer_display_works_minutes % 10;

		//display hours works


        if(gProcess_t.gTimer_display_works_hours > 99){
			gProcess_t.gTimer_display_works_hours=0;


		}
		
		glcd_t.number5_low = gProcess_t.gTimer_display_works_hours / 10;
		glcd_t.number5_high = gProcess_t.gTimer_display_works_hours / 10;


		glcd_t.number6_low = gProcess_t.gTimer_display_works_hours  % 10;
		glcd_t.number6_high = gProcess_t.gTimer_display_works_hours % 10;
		

	}
    else{
	 
		  glcd_t.number7_low = gProcess_t.gTimer_display_works_minutes / 10;
		  glcd_t.number7_high = gProcess_t.gTimer_display_works_minutes / 10;
		
						   
		  glcd_t.number8_low = gProcess_t.gTimer_display_works_minutes	% 10;
		  glcd_t.number8_high = gProcess_t.gTimer_display_works_minutes % 10;

	  
    }
	 

	

   }


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
static void Set_Timer_Timing_Lcd_Blink(void )
{
    if(gProcess_t.gTimer_set_timer_blink < 30){//300ms

	  glcd_t.number5_low =  gProcess_t.set_timer_timing_hours / 10 ;
      glcd_t.number5_high =  gProcess_t.set_timer_timing_hours / 10 ;

      glcd_t.number6_low  = gProcess_t.set_timer_timing_hours% 10; //
      glcd_t.number6_high =  gProcess_t.set_timer_timing_hours % 10; //
      
       //dispaly minutes 
      glcd_t.number7_low =   gProcess_t.set_timer_timing_minutes /10;
      glcd_t.number7_high =   gProcess_t.set_timer_timing_minutes /10;

      glcd_t.number8_low =   gProcess_t.set_timer_timing_minutes %10;
      glcd_t.number8_high =   gProcess_t.set_timer_timing_minutes %10;


    }
	else if(gProcess_t.gTimer_set_timer_blink > 29 && gProcess_t.gTimer_set_timer_blink > 61 ){
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
	else{
	 gProcess_t.gTimer_set_timer_blink =0;

    }

}

/*
*********************************************************************************************************
*
*	函 数 名: void Dissplay_Timer_Timing(uint8_t hours,uint8_t minutes)
*	功能说明: 到记时功能
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
static void Display_Timer_Timing(int8_t hours,int8_t minutes)
{
	 if(gProcess_t.gTimer_timer_Counter > 59){
	    gProcess_t.gTimer_timer_Counter =0;
		
		minutes -- ;
	
	    if(minutes <  0 ){
			 
		   hours -- ;
		   minutes =59;
         }

		
		
		 if(hours < 0 ){
		 
			
			hours=0;
			minutes=0;
			gkey_t.key_power = power_off;
			gkey_t.gTimer_power_off=0;
			
			
	      }



		 //display hours timing
	     glcd_t.number5_low = hours / 10;
		 glcd_t.number5_high = hours / 10;
	 
	 
		 glcd_t.number6_low = hours  % 10;
		 glcd_t.number6_high = hours % 10;
		 
	      //display minutes 
		 glcd_t.number7_low = minutes / 10;
		 glcd_t.number7_high = minutes / 10;
		 
							
		 glcd_t.number8_low = minutes  % 10;
		 glcd_t.number8_high = minutes % 10;
		    
     }

}


