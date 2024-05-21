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

           
             gkey_t.set_temp_value --;
			if( gkey_t.set_temp_value <20)  gkey_t.set_temp_value =40;
	        else if( gkey_t.set_temp_value  >40) gkey_t.set_temp_value =40;

           if( gkey_t.set_temp_value  > 40) gkey_t.set_temp_value = 20;
				
			   glcd_t.number3_low =  gkey_t.set_temp_value  / 10 ;
            glcd_t.number3_high =  gkey_t.set_temp_value  / 10 ;
			   glcd_t.number4_low  = gkey_t.set_temp_value  % 10; //
            glcd_t.number4_high =  gkey_t.set_temp_value  % 10; //

            gkey_t.gTimer_set_temp_value=0;

            Disp_SetTemp_Value(gkey_t.set_temp_value);

         break;

         case mode_set_timer: //set timer timing value 

              gkey_t.gTimer_disp_set_timer = 0; 

              gProcess_t.set_timer_timing_minutes =0;

              gProcess_t.set_timer_timing_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(gProcess_t.set_timer_timing_hours < 0){//if(run_t.dispTime_minutes < 0){

				    gProcess_t.set_timer_timing_hours =24;//run_t.dispTime_hours --;
					
					
				}
            

             //display hours
                  glcd_t.number5_low =  gProcess_t.set_timer_timing_hours / 10 ;
                  glcd_t.number5_high =  gProcess_t.set_timer_timing_hours / 10 ;

                  glcd_t.number6_low  = gProcess_t.set_timer_timing_hours % 10; //
                  glcd_t.number6_high =  gProcess_t.set_timer_timing_hours % 10; //
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

        
        gkey_t.set_temp_value  ++;
        if(gkey_t.set_temp_value  < 20){
            gkey_t.set_temp_value =20;
        }

        if(gkey_t.set_temp_value  > 40)gkey_t.set_temp_value = 20;

        glcd_t.number3_low = gkey_t.set_temp_value  / 10 ;
        glcd_t.number3_high = gkey_t.set_temp_value  / 10 ;
        glcd_t.number4_low  =gkey_t.set_temp_value  % 10; //
        glcd_t.number4_high = gkey_t.set_temp_value % 10; //

     
        gkey_t.gTimer_set_temp_value=0;

        Disp_SetTemp_Value(gkey_t.set_temp_value);

    break;

    case mode_set_timer: //set timer timing value 

         gkey_t.gTimer_disp_set_timer = 0; 

         gProcess_t.set_timer_timing_minutes=0;

        	gProcess_t.set_timer_timing_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
		   if(gProcess_t.set_timer_timing_hours > 24){ //if(run_t.dispTime_minutes > 59){

		          gProcess_t.set_timer_timing_hours=0;//run_t.dispTime_hours =0;
		                
             }
	//display hours
        glcd_t.number5_low =  gProcess_t.set_timer_timing_hours / 10 ;
        glcd_t.number5_high =  gProcess_t.set_timer_timing_hours / 10 ;

        glcd_t.number6_low  = gProcess_t.set_timer_timing_hours% 10; //
        glcd_t.number6_high =  gProcess_t.set_timer_timing_hours % 10; //
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





