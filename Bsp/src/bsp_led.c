#include "bsp_led.h"
#include "bsp.h"




static uint8_t Delay(int32_t count);

 uint32_t led_k;


void LED_Power_On(void)
{
    LED_POWER_KEY_SetHigh() ;


}
void LED_POWER_OFF(void)
{
	LED_POWER_KEY_SetLow() ;
}

void LED_Mode_On(void)
{
  LED_MODE_SetHigh();	

}
void LED_Mode_Off(void)
{

	LED_MODE_SetLow()	;


}


static uint8_t  Delay(int32_t count)
{
   
    if(count ==0 || count <0){
       return 1;
    
    }
    else{
       while(count){

        count--;
       }

       return 0;
   }
  

}


/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
void Breath_Led(void)
{
   
    static uint8_t flag,switch_flag,dec_led_flag;
    static uint8_t i,j,z;
    led_k++;
    
	
    if(led_k <25 && dec_led_flag==1){
        i++;
     if(switch_flag ==1 || i > 4){
        switch_flag =0;
      
        LED_POWER_OFF();
        flag = Delay(25-i);
       
      }
	  if(flag ==1){
        flag =0;
        LED_Power_On();
         j=i;
         if(j< 5){
           switch_flag = Delay(j);

         }
         switch_flag =1;
      

        }
      
    }
    else if(led_k < 25 && dec_led_flag==0){
        i++;
     if(switch_flag ==1 || i < 10){
        switch_flag =0;
        #if 0
        LED_POWER_OFF();
        LED_POWER_OFF();
        LED_Power_On();
        flag = Delay(30-i);
        #endif 
        LED_Power_On();
        flag = Delay(25-i);
        

      }
	  if(flag ==1){
        flag =0;
      
           if(j< 20){
           LED_POWER_OFF();
           switch_flag = Delay(i);

            }
            else{
             LED_POWER_OFF();
             LED_POWER_OFF();
             LED_POWER_OFF();
             LED_POWER_OFF();
             switch_flag = 1;


            }
        }
     }
    
    if(led_k > 25 && dec_led_flag==0){

    

//          z++; 
//      if(z<20){
//         LED_POWER_OFF();
//         
//      }
//      else{
    
		led_k=0;
         i=0;
         z=0;
         dec_led_flag=1;
        
     //  }
        
      }
      else if(led_k>25 && dec_led_flag==1){
        
     
       
        led_k=0;
         i=0;
         z=0;
         dec_led_flag=0;
        
//        if(z<20){
//         // LED_POWER_OFF();
//         LED_Power_On();
//         }
//         else{
//       
//		led_k=0;
//         i=0;
//         z=0;
//         dec_led_flag=0;
//        }
	}


    
	

}
	




void Backlight_On(void)
{
	BACKLIGHT_SetHigh();

}
void Backlight_Off(void)
{

	BACKLIGHT_SetLow();


}

