#include "bsp.h"



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
  
    LED_Mode_On();
    LED_Power_On();
    Backlight_On();
    Mainboard_Action_Fun();

}


void PowerOff_freeFun(void)
{
       
		Ultrasonic_Pwm_Stop();
		Plasma_Off();

        Fan_Stop();
		Backlight_Off();
		Lcd_Display_Off();
}

void PowerOff_Off_Led(void)
{
    LED_Mode_Off();
    LED_POWER_OFF();
    Ptc_Off();
    Ultrasonic_Pwm_Stop();
    Plasma_Off();



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
 void Mainboard_Action_Fun(void)
{
    Ptc_On();
	Ultrasonic_Pwm_Output();
	Fan_Run();
	Plasma_On();
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
    static uint8_t minutes_change_flag = 0xff;
    if(gpro_t.gTimer_works_counter_sencods > 59){

	  gpro_t.gTimer_works_counter_sencods=0;

	  gpro_t.disp_works_minutes_value++;

	if( gpro_t.disp_works_minutes_value > 59){ //1 hours
		gpro_t.disp_works_minutes_value=0;

	     gpro_t.disp_works_hours_value++;


//		glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
//		glcd_t.number7_high = gpro_t.disp_works_minutes_value / 10;
//
//
//		glcd_t.number8_low = gpro_t.disp_works_minutes_value  % 10;
//		glcd_t.number8_high = gpro_t.disp_works_minutes_value % 10;

		//display hours works
        if(gpro_t.disp_works_hours_value > 99){
               gpro_t.disp_works_hours_value=0;
        
        
          }


       
		
//		glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
//		glcd_t.number5_high = gpro_t.disp_works_hours_value / 10;
//
//
//		glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
//		glcd_t.number6_high = gpro_t.disp_works_hours_value % 10;

        
		

	}
    

   
    //Display_LCD_Works_Timing();

   }

    if(( minutes_change_flag != gpro_t.disp_works_minutes_value) || gpro_t.disp_timer_switch_time_flag >0){
 
         if(minutes_change_flag != gpro_t.disp_works_minutes_value)minutes_change_flag = gpro_t.disp_works_minutes_value;

         if(gpro_t.disp_timer_switch_time_flag >0) gpro_t.disp_timer_switch_time_flag=0;
         
        glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
		glcd_t.number5_high = gpro_t.disp_works_hours_value / 10;


		glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
		glcd_t.number6_high = gpro_t.disp_works_hours_value % 10;


        glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
		glcd_t.number7_high = gpro_t.disp_works_minutes_value / 10;


		glcd_t.number8_low = gpro_t.disp_works_minutes_value  % 10;
		glcd_t.number8_high = gpro_t.disp_works_minutes_value % 10;
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

       gctl_t.ai_flag = 1;

       LCD_Number_Ai_OneTwo_Humidity();
       
       glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
		glcd_t.number5_high = gpro_t.disp_works_hours_value / 10;


		glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
		glcd_t.number6_high = gpro_t.disp_works_hours_value % 10;
		

	
	 
		  glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
		  glcd_t.number7_high = gpro_t.disp_works_minutes_value / 10;
		
						   
		  glcd_t.number8_low = gpro_t.disp_works_minutes_value	% 10;
		  glcd_t.number8_high = gpro_t.disp_works_minutes_value % 10;

	    

	 
    Display_LCD_Works_Timing();


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

    // static uint8_t minutes_changed_flag = 0xff;
     if(gpro_t.gTimer_timer_Counter > 59){
	    gpro_t.gTimer_timer_Counter =0;
		
		gpro_t.set_timer_timing_minutes -- ;
    
	
	    if(gpro_t.set_timer_timing_minutes <  0 ){
			 
		   gpro_t.set_timer_timing_hours -- ;
		   gpro_t.set_timer_timing_minutes =59;
         }

		
		
		 if(gpro_t.set_timer_timing_hours < 0 ){
		 
			
    			gpro_t.set_timer_timing_hours=0;
    			gpro_t.set_timer_timing_minutes=0;
    			gkey_t.key_power = power_off;
                gpro_t.power_off_flag = 1;
    			//gkey_t.gTimer_power_off_run_times=0;
             
           
			
			
	      }

         //display hours timing
	     glcd_t.number5_low = gpro_t.set_timer_timing_hours / 10;
		 glcd_t.number5_high = gpro_t.set_timer_timing_hours / 10;
	 
	 
		 glcd_t.number6_low = gpro_t.set_timer_timing_hours   % 10;
		 glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10;
		 
	      //display minutes 
		 glcd_t.number7_low = gpro_t.set_timer_timing_minutes / 10;
		 glcd_t.number7_high = gpro_t.set_timer_timing_minutes / 10;
		 
							
		 glcd_t.number8_low = gpro_t.set_timer_timing_minutes   % 10;
		 glcd_t.number8_high = gpro_t.set_timer_timing_minutes % 10;

         LCD_Disp_Timer_Timing();
		    
     }

//     if(minutes_changed_flag != gpro_t.set_timer_timing_minutes){
//
//
//
//     }
     
 

}



void LCD_Disp_Timer_Timing(void)
{

   LCD_Number_FiveSix_Hours();
   LCD_Number_SevenEight_Minutes();

}

void LCD_Disp_Timer_Timing_Init(void)
{

     gctl_t.ai_flag = 0;

     LCD_Number_Ai_OneTwo_Humidity();
//display hours timing
    glcd_t.number5_low = gpro_t.set_timer_timing_hours / 10;
    glcd_t.number5_high = gpro_t.set_timer_timing_hours / 10;


    glcd_t.number6_low = gpro_t.set_timer_timing_hours   % 10;
    glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10;

    //display minutes 
    glcd_t.number7_low = gpro_t.set_timer_timing_minutes / 10;
    glcd_t.number7_high = gpro_t.set_timer_timing_minutes / 10;

    				
    glcd_t.number8_low = gpro_t.set_timer_timing_minutes   % 10;
    glcd_t.number8_high = gpro_t.set_timer_timing_minutes % 10;

    LCD_Disp_Timer_Timing();

}





/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   

*********************************************************************************/
void Display_WorksTimingr_Handler(uint8_t sel_item)
{

   static uint8_t switch_counter;
 
    switch(sel_item){

    case disp_works_timing :
   
       if(gctl_t.fan_warning ==0 && gctl_t.ptc_warning==0 ){

           gctl_t.ai_flag = 1; // AI DISPLAY AI ICON
       
               
            if(switch_counter>0){
               switch_counter =0;
              }
         
            Display_Works_Timing();
            

        }
        else{
            
          LCD_Fault_Numbers_Code();

        }
       
    break;
    
    case disp_timer_timing:
   

      if(gctl_t.fan_warning ==0 && gctl_t.ptc_warning==0 ){
            if(gkey_t.set_timer_timing_success ==1){
               gctl_t.ai_flag = 0; // don't  DISPLAY AI ICON
               Display_Timer_Timing();
             

            }
            else if(gkey_t.set_timer_timing_success == 0 ){ //&& gkey_t.gTimer_disp_switch_disp_mode > 3){

               
                gctl_t.ai_flag =0;
                LCD_Disp_Timer_Timing_Init();

                switch_counter ++;
                if( switch_counter> 20){
                    gpro_t.disp_timer_switch_time_flag ++ ;
                 gkey_t.key_mode = disp_works_timing;
                  

               }
               
            }
       }
        else{

            LCD_Fault_Numbers_Code();

        }


    
    break;


    case mode_set_timer:
    
        Set_Timer_Timing_Lcd_Blink();//(gpro_t.set_timer_timing_hours,gpro_t.set_timer_timing_minutes);
       
        if(gkey_t.gTimer_disp_set_timer > 1){

            if(gpro_t.set_timer_timing_hours == 0 && gpro_t.set_timer_timing_minutes==0){

                gkey_t.set_timer_timing_success = 0;

                gctl_t.ai_flag = 1;
                gkey_t.key_mode =disp_works_timing;
                gpro_t.first_disp_work_time =0; //at once display works times of value ;
                gkey_t.key_add_dec_mode = set_temp_value_item;
                LCD_Disp_Works_Timing_Init();
                 
                
            }
            else{
                gkey_t.set_timer_timing_success = 1;
                gpro_t.gTimer_timer_Counter =0; //start recoder timer timing is "0",from "0" start

                gctl_t.ai_flag = 0;
              
                gkey_t.key_mode = disp_timer_timing;
                gkey_t.key_add_dec_mode = set_temp_value_item;
                
                LCD_Disp_Timer_Timing_Init();

                if(wifi_link_net_state()==1){
                    MqttData_Publish_SetState(2); //timer model  = 2, works model = 1
                    HAL_Delay(200);
                }
               
            }
        }

       break;
    }
}


void Record_WorksOr_Timer_Timing_DonotDisp_Handler(void)
{
    if(gkey_t.key_mode == disp_timer_timing ){
    if(gpro_t.gTimer_works_counter_sencods > 59 ){
    
          gpro_t.gTimer_works_counter_sencods=0;
    
          gpro_t.disp_works_minutes_value++;
    
        if( gpro_t.disp_works_minutes_value > 59){ //1 hours
            gpro_t.disp_works_minutes_value=0;
    
             gpro_t.disp_works_hours_value++;
    
    
            if(gpro_t.disp_works_hours_value > 99){
                gpro_t.disp_works_hours_value=0;
    
    
            }
            
        }
        }
     }
     else if(gkey_t.key_mode == disp_works_timing){

     if( gkey_t.set_timer_timing_success == 1){
     if(gpro_t.gTimer_timer_Counter > 59){
	    gpro_t.gTimer_timer_Counter =0;
		
		gpro_t.set_timer_timing_minutes -- ;
    
	
	    if(gpro_t.set_timer_timing_minutes <  0 ){
			 
		   gpro_t.set_timer_timing_hours -- ;
		   gpro_t.set_timer_timing_minutes =59;
         }

		
		
		 if(gpro_t.set_timer_timing_hours < 0 ){
		 
			
			gpro_t.set_timer_timing_hours=0;
    		gpro_t.set_timer_timing_minutes=0;
    		gkey_t.key_power = power_off;
            gpro_t.power_off_flag = 1;
    			//gkey_t.gTimer_power_off_run_times=0;
           
            
			
	      }

        }
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
void Set_Timer_Timing_Lcd_Blink(void)
{

    #if 0


    if(gpro_t.gTimer_disp_set_timer_blink < 3){//3* 100ms

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
	else if(gpro_t.gTimer_disp_set_timer_blink > 2 && gpro_t.gTimer_disp_set_timer_blink < 6){
	


      glcd_t.number5_low =  gpro_t.set_timer_timing_hours  / 10 ;
      glcd_t.number5_high =  gpro_t.set_timer_timing_hours  / 10 ;

      glcd_t.number6_low  = gpro_t.set_timer_timing_hours% 10; //
      glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10; //
      
       //dispaly minutes 
      glcd_t.number7_low =  0;
      glcd_t.number7_high =  0;

      glcd_t.number8_low = 0;
      glcd_t.number8_high =   0;
    
       
	}
	else if(gpro_t.gTimer_disp_set_timer_blink > 5){
	  gpro_t.gTimer_disp_set_timer_blink =0;
      
      
    }

  	LCD_Disp_Timer_Timing();
    #else

     glcd_t.number5_low =  0x0A ;
      glcd_t.number5_high =  0x0A ;

      glcd_t.number6_low  =  0x0A; //
      glcd_t.number6_high =  0x0A; //
      
       //dispaly minutes 
      glcd_t.number7_low =   0x0A;
      glcd_t.number7_high =   0x0A;

      glcd_t.number8_low =   0x0A;
      glcd_t.number8_high =   0x0A;
      LCD_Disp_Timer_Timing();
      HAL_Delay(100);

      glcd_t.number5_low =  gpro_t.set_timer_timing_hours  / 10 ;
      glcd_t.number5_high =  gpro_t.set_timer_timing_hours  / 10 ;

      glcd_t.number6_low  = gpro_t.set_timer_timing_hours% 10; //
      glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10; //
      
       //dispaly minutes 
      glcd_t.number7_low =  0;
      glcd_t.number7_high =  0;

      glcd_t.number8_low = 0;
      glcd_t.number8_high =   0;
      LCD_Disp_Timer_Timing();
      HAL_Delay(100);
    

    #endif 

    

}






