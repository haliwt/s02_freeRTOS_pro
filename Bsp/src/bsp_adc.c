#include "bsp_adc.h"
#include "bsp.h"
#include "adc.h"

static uint16_t Get_Adc_Channel(uint32_t ch) ;

static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);

uint16_t fan_detect_voltage;

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Channel(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

	ADC1_ChanConf.Channel=ch;                                   //Í¨µÀ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_1;                                    //第一个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //²ÉÑùÊ±¼ä               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Channel(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 



void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times)
{
    uint16_t adcx,ptc_temp_voltage;

    #if 1
	
	adcx = Get_Adc_Average(channel,times);

    ptc_temp_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311


	#ifdef DEBUG
      printf("ptc= %d",run_t.ptc_temp_voltage);
	#endif 

 

    #endif 


	if(ptc_temp_voltage < 373 || ptc_temp_voltage ==373){ //87 degree
  
	    gctl_t.plasma_flag = 0; //turn off
	    Ptc_Off(); //turn off

        gctl_t.ptc_warning = 1;
        
        Buzzer_Ptc_Error_Sound();

        if(wifi_link_net_state() ==1){

        Publish_Data_Warning(ptc_temp_warning,warning);
	     HAL_Delay(200);//osDelay(300);//HAL_Delay(350);
 
 
        MqttData_Publish_SetPtc(0);
		 HAL_Delay(100);//osDelay(300);//HAL_Delay(350);  

        }
   	      
   }
}


/*****************************************************************
	*
	*Function Name: void Judge_PTC_Temperature_Value(void)
	*Function: PTC adc read voltage
	*Input Ref: NO
	*Return Ref: No
	*
	*
*****************************************************************/
//void Judge_PTC_Temperature_Value(void)
//{
//  
//  if(run_t.ptc_temp_voltage < 373 || run_t.ptc_temp_voltage ==373){ //87 degree
//  
//	    gctl_t.plasma_flag = 0; //turn off
//	    PTC_SetLow(); //turn off
//        Buzzer_Ptc_Error_Sound();
//   	      
//   }
//   
//}

/*****************************************************************
	*
	*Function Name: void Get_Fan_Adc_Fun(uint8_t channel,uint8_t times)
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?, hexadecimal of average
	*Return Ref: No
	*
	*
*****************************************************************/
void Get_Fan_Adc_Fun(uint32_t channel,uint8_t times)
{
	uint16_t adc_fan_hex;//,fan_detect_voltage;
	
	static uint8_t detect_error_times;
	
	adc_fan_hex = Get_Adc_Average(channel,times);

    fan_detect_voltage  =(uint16_t)((adc_fan_hex * 3300)/4096); //amplification 1000 ,3.111V -> 3111
	
   


    #if BALL_FAN
      if(fan_detect_voltage >   520  &&  fan_detect_voltage < 1400){ //
                   detect_error_times=0;
    		   #ifdef DEBUG
                     printf("adc= %d",run_t.fan_detect_voltage);
    		   #endif 
                   gctl_t.fan_warning = 0;
     }


    #else

	if(fan_detect_voltage >300 &&  fan_detect_voltage < 1400){
           detect_error_times=0;
		   #ifdef DEBUG
             printf("adc= %d",run_t.fan_detect_voltage);
		   #endif 
           gctl_t.fan_warning = 0;
    }
   #endif 
   else{

	          
		if(detect_error_times >0){
			detect_error_times=0;
		 gctl_t.fan_warning = 1;

		  Buzzer_Fan_Error_Sound();

           wifi_t.set_wind_speed_value = 2;
          
       
          if(wifi_link_net_state() ==1){
           Publish_Data_Warning(fan_warning,warning);
	       HAL_Delay(100);//osDelay(350);//HAL_Delay(350);

           MqttData_Publis_SetFan(0);
	       HAL_Delay(200);//osDelay(350);//HAL_Delay(350);


          LCD_Fault_Numbers_Code();

            }

          }
		detect_error_times++;

        
   }
}

    


