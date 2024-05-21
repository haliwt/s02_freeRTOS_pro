#include "bsp_key.h"
#include "bsp.h"

key_fun_t gkey_t;

uint8_t set_up_temperature_value;

/***************************************************************************
    *
    *Function Name:void Dec_Key_Fun(uint8_t cmd)
    *Function : key down function 
    *Input Ref: run cmd
    *Return Ref : NO
    *
***************************************************************************/
void Dec_Key_Fun(uint8_t cmd)
{
    switch(cmd){

         case set_temp_value_item: //set temperature 

           
            set_up_temperature_value--;
			if(set_up_temperature_value<20) set_up_temperature_value=40;
	        else if(set_up_temperature_value >40)set_up_temperature_value=40;

           if(set_up_temperature_value > 40)set_up_temperature_value= 20;
				
			   glcd_t.number3_low = set_up_temperature_value / 10 ;
            glcd_t.number3_high = set_up_temperature_value / 10 ;
			   glcd_t.number4_low  =set_up_temperature_value % 10; //
            glcd_t.number4_high = set_up_temperature_value % 10; //

            gkey_t.set_temp_value = set_up_temperature_value;
            gkey_t.gTimer_set_temp_value=0;

            Disp_SetTemp_Value(gkey_t.set_temp_value);

         break;

         case mode_set_timer: //set timer timing value 

              gkey_t.gTimer_disp_set_timer = 0; 
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

                

          LCD_Disp_Timer_Timing();

         break;
 
         }
  

}

/***************************************************************************
    *
    *Function Name:void Dec_Key_Fun(uint8_t cmd)
    *Function : key down function 
    *Input Ref: run cmd
    *Return Ref : NO
    *
***************************************************************************/
void Add_Key_Fun(uint8_t cmd)
{

    
    switch(cmd){
        
    case set_temp_value_item:  //set temperature value 

        
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
        gkey_t.gTimer_set_temp_value=0;

        Disp_SetTemp_Value(gkey_t.set_temp_value);

    break;

    case mode_set_timer: //set timer timing value 

         gkey_t.gTimer_disp_set_timer = 0; 

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

       // gkey_t.gTimer_disp_set_timer_blink =0;
        
        LCD_Disp_Timer_Timing();
     break;
        
    }

 }





