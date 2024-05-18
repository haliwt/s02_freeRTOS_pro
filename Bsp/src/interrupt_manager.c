#include "interrupt_manager.h"
#include "bsp.h"

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function call back function
	*
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t tm0,tm1,tm2;
    
   if(htim->Instance==TIM17){
    
    tm0++;  //10ms
	tm1++;
    gProcess_t.gTimer_normal_display_lcd++;
    glcd_t.gTimer_lcd_blink++;
    
	glcd_t.gTimer_fan_blink++;
	gProcess_t.gTimer_set_timer_blink ++;
	
    if(tm1>9){ //10*10 = 100ms 
       tm1=0;
       glcd_t.gTimer_colon_blink++ ;
	  
    }

    if(tm0>99){ //100 *10ms = 1000ms = 1s
		tm0=0;
		tm2++;

		
		gkey_t.gTimer_key_temp_timing++;
		glcd_t.gTimer_error_times++;
	    gctl_t.gTimer_prcoess_iwdg++;
		gProcess_t.gTimer_run_adc++ ;
		gProcess_t.gTimer_run_dht11++;
        gProcess_t.gTimer_normal_display_lcd ++;
		gProcess_t.gTimer_run_one_mintue ++;
		gkey_t.gTimer_power_off++;
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







