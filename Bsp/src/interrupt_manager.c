#include "interrupt_manager.h"
#include "bsp.h"

uint8_t inputBuf[1];
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t state_uart1;
    
   
    //wifi usart1 --wifi 
    if(huart->Instance==USART2)
    {
       #if 0 
	  if(wifi_t.linking_tencent_cloud_doing  ==1){ //link tencent netware of URL

			wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
			wifi_t.wifi_uart_counter++;

			if(*wifi_t.usart1_dataBuf==0X0A) // 0x0A = "\n"
			{
				//wifi_t.usart2_rx_flag = 1;
				Wifi_Rx_Link_Net_InputInfo_Handler();
				wifi_t.wifi_uart_counter=0;
			}

	      } 
		  else{

		         if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
					wifi_t.wifi_uart_counter++;
				}
				else if(wifi_t.get_rx_auto_repeat_net_enable ==1){

					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
					wifi_t.wifi_uart_counter++;

					if(*wifi_t.usart1_dataBuf==0X0A) // 0x0A = "\n"
					{
						
						Wifi_Rx_Auto_Link_Net_Handler();
						wifi_t.wifi_uart_counter=0;
					}


				}
				else{
					Subscribe_Rx_Interrupt_Handler();

				}
	      }
	 
	  #endif 
//	__HAL_UART_CLEAR_NEFLAG(&huart2);
	__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	//__HAL_UART_CLEAR_TXFECF(&huart2);
	   UART_Start_Receive_IT(&huart2,inputBuf,1);
     
	}


 }

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
        
        
       glcd_t.gTimer_fan_blink++;
    	
    }
	
    if(tm3>99){ //10*10 = 100ms 
       tm3=0;
       glcd_t.gTimer_colon_blink++ ;
       gProcess_t.gTimer_disp_set_timer_blink ++; //set timer timing counter is digital blink.
       glcd_t.gTimer_set_temp_blink++;   //set temperature value digital numbers blink .

       
	  
    }

    if(tm0>999){ //100 *10ms = 1000ms = 1s
		tm0=0;
		tm2++;
      
   
		
		
		glcd_t.gTimer_error_times++;
	    gctl_t.gTimer_prcoess_iwdg++;
        gctl_t.gTimer_wifi_blink++;
        
        //key timer
        gkey_t.gTimer_disp_set_timer++;
        
        gkey_t.gTimer_power_off++;
        gkey_t.gTimer_set_temp_value++;         //add or dec key set temperature value counter
  

      

        //process gtimer
		gProcess_t.gTimer_run_adc++ ;
		gProcess_t.gTimer_run_dht11++;
   
		gProcess_t.gTimer_run_one_mintue ++;
		
		gProcess_t.gTimer_works_counter++;

        gProcess_t.gTimer_display_dht11_value ++;
        gProcess_t.gTimer_timer_Counter++;   //timer timing counter value 

        

       
		

     
		
		if(tm2 > 59){ //60s = 1 minutes
			tm2 =0;
			gProcess_t.gTimer_run_total++;
			gProcess_t.gTimer_run_time_out ++ ;
			

		}
	}
	
   }


}







