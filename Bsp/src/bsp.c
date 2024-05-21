#include "bsp.h"


BSP_process_t gProcess_t;
static uint8_t Works_Time_Out(void);
static void Mainboard_Action_Fun(void);
static void Mainboard_Fun_Stop(void);
static void Process_Dynamical_Action(void);

//void Set_Timer_Timing_Lcd_Blink(void );

//static void Display_Timer_Timing(int8_t hours,int8_t minutes);

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


void PowerOff_freeFun(void)
{
      LED_Mode_Off();
	   
	   
        Ptc_Off();
		Ultrasonic_Pwm_Stop();
		Plasma_Off();

        Fan_Stop();
		Backlight_Off();
		Lcd_Display_Off();
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
uint8_t fan_continue_flag;
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
void Display_Works_Timing(void)
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
	 
    Display_LCD_Works_Timing();
    

   }

   

}

static void Display_LCD_Works_Timing(void)
{


    LCD_Number_FiveSix_Hours();
    LCD_Number_SevenEight_Minutes();


}

void LCD_Disp_Works_Timing_Init(void)
{

    if(gProcess_t.gTimer_display_works_minutes > 59){ //1 hours
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
	 
    Display_LCD_Works_Timing();


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
void Set_Timer_Timing_Lcd_Blink(uint8_t hours,uint8_t minutes)
{
    if(gProcess_t.gTimer_disp_set_timer_blink < 30){//300ms

	  glcd_t.number5_low =  hours / 10 ;
      glcd_t.number5_high =  hours / 10 ;

      glcd_t.number6_low  = minutes% 10; //
      glcd_t.number6_high =  minutes % 10; //
      
       //dispaly minutes 
      glcd_t.number7_low =  minutes /10;
      glcd_t.number7_high =   minutes /10;

      glcd_t.number8_low =  minutes %10;
      glcd_t.number8_high =    minutes %10;


    }
	else if(gProcess_t.gTimer_disp_set_timer_blink > 29 && gProcess_t.gTimer_disp_set_timer_blink > 61 ){
//	  glcd_t.number5_low =  0x0A ;
//      glcd_t.number5_high =  0x0A ;
//
//      glcd_t.number6_low  =  0x0A; //
//      glcd_t.number6_high =  0x0A; //
//      
//       //dispaly minutes 
//      glcd_t.number7_low =   0x0A;
//      glcd_t.number7_high =   0x0A;
//
//      glcd_t.number8_low =   0x0A;
//      glcd_t.number8_high =   0x0A;
//

	}
	else{
	 gProcess_t.gTimer_disp_set_timer_blink =0;

    }

    LCD_Disp_Timer_Timing();

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
void Display_Timer_Timing(void)
{

     if(gProcess_t.gTimer_timer_Counter > 59){
	    gProcess_t.gTimer_timer_Counter =0;
		
		gProcess_t.set_timer_timing_minutes -- ;
    
	
	    if(gProcess_t.set_timer_timing_minutes <  0 ){
			 
		   gProcess_t.set_timer_timing_hours -- ;
		   gProcess_t.set_timer_timing_minutes =59;
         }

		
		
		 if(gProcess_t.set_timer_timing_hours < 0 ){
		 
			
			gProcess_t.set_timer_timing_hours=0;
			gProcess_t.set_timer_timing_minutes=0;
			gkey_t.key_power = power_off;
			gkey_t.gTimer_power_off=0;
			
			
	      }



		 //display hours timing
	     glcd_t.number5_low = gProcess_t.set_timer_timing_hours / 10;
		 glcd_t.number5_high = gProcess_t.set_timer_timing_hours / 10;
	 
	 
		 glcd_t.number6_low = gProcess_t.set_timer_timing_hours   % 10;
		 glcd_t.number6_high = gProcess_t.set_timer_timing_hours % 10;
		 
	      //display minutes 
		 glcd_t.number7_low = gProcess_t.set_timer_timing_minutes / 10;
		 glcd_t.number7_high = gProcess_t.set_timer_timing_minutes / 10;
		 
							
		 glcd_t.number8_low = gProcess_t.set_timer_timing_minutes   % 10;
		 glcd_t.number8_high = gProcess_t.set_timer_timing_minutes % 10;

         LCD_Disp_Timer_Timing();
		    
     }
     
    //LCD_Disp_Timer_Timing();

}


void LCD_Disp_Timer_Timing(void)
{

   LCD_Number_FiveSix_Hours();
   LCD_Number_SevenEight_Minutes();

}

void LCD_Disp_Timer_Timing_Init(void)
{

    //display hours timing
    glcd_t.number5_low = gProcess_t.set_timer_timing_hours / 10;
    glcd_t.number5_high = gProcess_t.set_timer_timing_hours / 10;


    glcd_t.number6_low = gProcess_t.set_timer_timing_hours   % 10;
    glcd_t.number6_high = gProcess_t.set_timer_timing_hours % 10;

    //display minutes 
    glcd_t.number7_low = gProcess_t.set_timer_timing_minutes / 10;
    glcd_t.number7_high = gProcess_t.set_timer_timing_minutes / 10;

    				
    glcd_t.number8_low = gProcess_t.set_timer_timing_minutes   % 10;
    glcd_t.number8_high = gProcess_t.set_timer_timing_minutes % 10;

    LCD_Disp_Timer_Timing();

}

/**********************************************************************************************************
*
*	函 数 名: void Dissplay_Timer_Timing(uint8_t hours,uint8_t minutes)
*	功能说明: 到记时功能
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************/ 
void Run_Main_Handler(void)
{


    if(gProcess_t.gTimer_normal_display_lcd >3 && gProcess_t.gTimer_normal_display_lcd<5){

             Process_Dynamical_Action();


     }
     else if(gProcess_t.gTimer_normal_display_lcd  > 9){

          gProcess_t.gTimer_normal_display_lcd=0;
            // Update_DHT11_Value();

           
              Update_DHT11_Value();
    			  	
    	     Disp_HumidityTemp_Value();

    	          

    }
    Disip_Wifi_Icon_State(  );

    
     

   

}
