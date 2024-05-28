#include "bsp_key.h"
#include "bsp.h"

key_fun_t gkey_t;



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

           
             gctl_t.gSet_temperature_value  --;
			if( gctl_t.gSet_temperature_value  <20)  gctl_t.gSet_temperature_value  =40;
	        else if( gctl_t.gSet_temperature_value   >40) gctl_t.gSet_temperature_value  =40;

           if( gctl_t.gSet_temperature_value   > 40) gctl_t.gSet_temperature_value  = 20;
				
			   glcd_t.number3_low =  gctl_t.gSet_temperature_value   / 10 ;
            glcd_t.number3_high =  gctl_t.gSet_temperature_value   / 10 ;
			   glcd_t.number4_low  = gctl_t.gSet_temperature_value   % 10; //
            glcd_t.number4_high =  gctl_t.gSet_temperature_value   % 10; //

            gkey_t.gTimer_set_temp_value=0;
            gpro_t.set_temp_confirm = 0;
            g_tMsg.key_add_dec_pressed_flag = 1;

            Disp_SetTemp_Value(gctl_t.gSet_temperature_value );

         break;

         case mode_set_timer: //set timer timing value 

              gkey_t.gTimer_disp_set_timer = 0; 

              gpro_t.set_timer_timing_minutes =0;

              gpro_t.set_timer_timing_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(gpro_t.set_timer_timing_hours < 0){//if(run_t.dispTime_minutes < 0){

				    gpro_t.set_timer_timing_hours =24;//run_t.dispTime_hours --;
					
					
				}
            

             //display hours
                  glcd_t.number5_low =  gpro_t.set_timer_timing_hours / 10 ;
                  glcd_t.number5_high =  gpro_t.set_timer_timing_hours / 10 ;

                  glcd_t.number6_low  = gpro_t.set_timer_timing_hours % 10; //
                  glcd_t.number6_high =  gpro_t.set_timer_timing_hours % 10; //
                   //dispaly minutes 
                  glcd_t.number7_low =  0;
                  glcd_t.number7_high =   0;

                  glcd_t.number8_low = 0;
                  glcd_t.number8_high =   0;

                

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

        
        gctl_t.gSet_temperature_value   ++;
        if(gctl_t.gSet_temperature_value   < 20){
            gctl_t.gSet_temperature_value  =20;
        }

        if(gctl_t.gSet_temperature_value   > 40)gctl_t.gSet_temperature_value  = 20;

        glcd_t.number3_low = gctl_t.gSet_temperature_value   / 10 ;
        glcd_t.number3_high = gctl_t.gSet_temperature_value   / 10 ;
        glcd_t.number4_low  =gctl_t.gSet_temperature_value   % 10; //
        glcd_t.number4_high = gctl_t.gSet_temperature_value  % 10; //

     
        gkey_t.gTimer_set_temp_value=0;
        gpro_t.set_temp_confirm = 0;
         g_tMsg.key_add_dec_pressed_flag = 1;
        Disp_SetTemp_Value(gctl_t.gSet_temperature_value );

    break;

    case mode_set_timer: //set timer timing value 

         gkey_t.gTimer_disp_set_timer = 0; 

         gpro_t.set_timer_timing_minutes=0;

        	gpro_t.set_timer_timing_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
		   if(gpro_t.set_timer_timing_hours > 24){ //if(run_t.dispTime_minutes > 59){

		          gpro_t.set_timer_timing_hours=0;//run_t.dispTime_hours =0;
		                
             }
	//display hours
        glcd_t.number5_low =  gpro_t.set_timer_timing_hours / 10 ;
        glcd_t.number5_high =  gpro_t.set_timer_timing_hours / 10 ;

        glcd_t.number6_low  = gpro_t.set_timer_timing_hours% 10; //
        glcd_t.number6_high =  gpro_t.set_timer_timing_hours % 10; //
        //dispaly minutes 
        glcd_t.number7_low =  0;
        glcd_t.number7_high = 0;

        glcd_t.number8_low = 0;
        glcd_t.number8_high =  0;

       // gkey_t.gTimer_disp_set_timer_blink =0;
        
        LCD_Disp_Timer_Timing();
     break;
        
    }

 }





