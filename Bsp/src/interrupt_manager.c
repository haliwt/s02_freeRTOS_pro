#include "interrupt_manager.h"
#include "bsp.h"

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function call back function
	*timer 1ms 
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t tm1,tm2,tm3;
  static uint16_t tm0;
    
   if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	tm1++;
    tm3++;

    if(tm1>9){
        tm1=0;
        gProcess_t.gTimer_normal_display_lcd++;
        glcd_t.gTimer_lcd_blink++;
        
    	glcd_t.gTimer_fan_blink++;
    	gProcess_t.gTimer_set_timer_blink ++;
    }
	
    if(tm3>99){ //10*10 = 100ms 
       tm3=0;
       glcd_t.gTimer_colon_blink++ ;
	  
    }

    if(tm0>999){ //100 *10ms = 1000ms = 1s
		tm0=0;
		tm2++;
      
         glcd_t.gTimer_fan_blink++;
		
		gkey_t.gTimer_key_temp_timing++;
		glcd_t.gTimer_error_times++;
	    gctl_t.gTimer_prcoess_iwdg++;
        gkey_t.gTimer_power_off++;

        //process gtimer
		gProcess_t.gTimer_run_adc++ ;
		gProcess_t.gTimer_run_dht11++;
        gProcess_t.gTimer_normal_display_lcd ++;
		gProcess_t.gTimer_run_one_mintue ++;
		
		gProcess_t.gTimer_works_counter++;
		gProcess_t.gTimer_set_timer_blink++;
		
		if(tm2 > 59){ //60s = 1 minutes
			tm2 =0;
			gProcess_t.gTimer_run_total++;
			gProcess_t.gTimer_run_time_out ++ ;
			

		}
	}
	
   }


}







