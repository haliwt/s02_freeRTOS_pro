#include "bsp_key.h"
#include "bsp.h"

key_fun_t gkey_t;

uint8_t key_mode_change;
uint8_t set_up_temperature_value ;
int8_t set_timer_dispTime_minutes;
int8_t set_timer_dispTime_hours;

/*
  * key interrupt call back function 

*/

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{

 
   switch(GPIO_Pin){

   case KEY_POWER_Pin:

      if(gkey_t.key_power == power_off){
	  	   gkey_t.key_sound_flag = key_sound;
           gkey_t.key_power = power_on;

      }
	  else{
	  	  gkey_t.key_sound_flag = key_sound;
          gkey_t.key_power = power_off;
	      gkey_t.gTimer_power_off = 0;

	  }
   break;

   case KEY_MODE_Pin:
   if(gkey_t.key_power == power_on){
   	  gkey_t.key_sound_flag = key_sound;
      gkey_t.key_mode_times = gkey_t.key_mode_times ^ 0x01;
      if(gkey_t.key_mode_times == 1){
          gkey_t.key_mode = mode_set_timer;
          key_mode_change = mode_set_timer;
      }
      else{

         gkey_t.key_mode = mode_confirm; //如果我只按一次那？，经过一段时间推出，恢复 
         key_mode_change = 0;
      }
   }
   break;


   case KEY_UP_Pin:
       if(gkey_t.key_power == power_on){
	   	
          gkey_t.key_sound_flag = key_sound;
		  switch(key_mode_change){

		    case 0:  //set temperature value 
		 
		      gkey_t.key_select = mode_set_temp;
            set_up_temperature_value ++;
	         if(set_up_temperature_value < 20){
				    set_up_temperature_value=20;
				}
				
				if(set_up_temperature_value > 40)set_up_temperature_value= 20;
				
			   glcd_t.number3_low = set_up_temperature_value / 10 ;
            glcd_t.number3_high = set_up_temperature_value / 10 ;
			   glcd_t.number4_low  =set_up_temperature_value % 10; //
            glcd_t.number4_high = set_up_temperature_value % 10; //

            gkey_t.set_temp_value = set_up_temperature_value;
            gkey_t.gTimer_key_temp_timing=0;
			
			break;

			case mode_set_timer: //set timer timing value 
			
				 if(gProcess_t.set_timer_timing_hours!=24)
				 		 gProcess_t.set_timer_timing_minutes=   gProcess_t.set_timer_timing_minutes + 30;
				 else if(gProcess_t.set_timer_timing_hours ==24)
				 	     gProcess_t.set_timer_timing_minutes =   gProcess_t.set_timer_timing_minutes + 60;

			    if( gProcess_t.set_timer_timing_minutes >59){
					     gProcess_t.set_timer_timing_hours++;
		             if(gProcess_t.set_timer_timing_hours ==24){
						     gProcess_t.set_timer_timing_minutes=0;
					}
					else if(gProcess_t.set_timer_timing_hours >24){

					   gProcess_t.set_timer_timing_hours =0;
					    gProcess_t.set_timer_timing_minutes=0;


					}
					else{

					  // set_timer_dispTime_minutes=0;
                 gProcess_t.set_timer_timing_minutes =0;


					}
						
			     }
                  //display hours
                  glcd_t.number5_low =  gProcess_t.set_timer_timing_hours / 10 ;
                  glcd_t.number5_high =  gProcess_t.set_timer_timing_hours / 10 ;

                  glcd_t.number6_low  = gProcess_t.set_timer_timing_hours% 10; //
                  glcd_t.number6_high =  gProcess_t.set_timer_timing_hours % 10; //
                   //dispaly minutes 
                  glcd_t.number7_low =   gProcess_t.set_timer_timing_minutes /10;
                  glcd_t.number7_high =   gProcess_t.set_timer_timing_minutes /10;

                  glcd_t.number8_low =   gProcess_t.set_timer_timing_minutes %10;
                  glcd_t.number8_high =   gProcess_t.set_timer_timing_minutes %10;
                 
                 gkey_t.gTimer_set_timer_blink =0;
            }
         
       }
   break;

   case KEY_DOWN_Pin:
     if(gkey_t.key_power == power_on){

	     gkey_t.key_sound_flag = key_sound;
        
         switch(key_mode_change){

         case 0: //set temperature 
       
		 gkey_t.key_select = mode_set_temp;
         set_up_temperature_value--;
			if(set_up_temperature_value<20) set_up_temperature_value=40;
	        else if(set_up_temperature_value >40)set_up_temperature_value=40;

           if(set_up_temperature_value > 40)set_up_temperature_value= 20;
				
			   glcd_t.number3_low = set_up_temperature_value / 10 ;
            glcd_t.number3_high = set_up_temperature_value / 10 ;
			   glcd_t.number4_low  =set_up_temperature_value % 10; //
            glcd_t.number4_high = set_up_temperature_value % 10; //

            gkey_t.set_temp_value = set_up_temperature_value;
            gkey_t.gTimer_key_temp_timing=0;

         break;

         case mode_set_timer: //set timer timing value 
             gProcess_t.set_timer_timing_minutes =   gProcess_t.set_timer_timing_minutes-30;
		        if( gProcess_t.set_timer_timing_minutes < 0){
					 gProcess_t.set_timer_timing_hours--;
                   if( gProcess_t.set_timer_timing_hours <0){
                         
				       gProcess_t.set_timer_timing_hours=24;
					   gProcess_t.set_timer_timing_minutes=0;

				   }
				   else{

				      gProcess_t.set_timer_timing_minutes =30;
               }
				  
				}

             //display hours
                  glcd_t.number5_low =  gProcess_t.set_timer_timing_hours / 10 ;
                  glcd_t.number5_high =  gProcess_t.set_timer_timing_hours / 10 ;

                  glcd_t.number6_low  = gProcess_t.set_timer_timing_hours % 10; //
                  glcd_t.number6_high =  gProcess_t.set_timer_timing_hours % 10; //
                   //dispaly minutes 
                  glcd_t.number7_low =   gProcess_t.set_timer_timing_minutes /10;
                  glcd_t.number7_high =   gProcess_t.set_timer_timing_minutes /10;

                  glcd_t.number8_low =  gProcess_t.set_timer_timing_minutes %10;
                  glcd_t.number8_high =   gProcess_t.set_timer_timing_minutes %10;

                 gkey_t.gTimer_set_timer_blink =0;

         break;
 
         }
     }

   break;


   }



}




