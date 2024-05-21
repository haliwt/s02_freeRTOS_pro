#include "bsp.h"
#include "queue.h"

uint8_t recieve_flag;
uint8_t receive_key_message;
uint8_t receive_task_start;
uint16_t key_long_counter;
/*
**********************************************************************************************************
											宏定义
**********************************************************************************************************
*/
#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define RUN_POWER_4         (1 << 4)
#define RUN_MODE_5          (1 << 5)
#define RUN_DEC_6           (1 << 6)
#define RUN_ADD_7           (1 << 7)

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
//static void vTaskTaskUserIF(void *pvParameters);
//static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
//static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;


static void display_works_timer_timing_fun(uint8_t sel_item);

static void mode_long_short_key_fun(void);


typedef struct Msg
{
    uint8_t add_dec_key_input_flag;
    uint8_t modekey_detect;
    uint8_t set_timer_timing_success;
    
    uint8_t key_mode;
    uint8_t disp_timer_works_switch_flag;
	uint8_t usData[2];
	uint8_t ulData[2];

    uint16_t ucMessageID;
 
}MSG_T;

MSG_T   g_tMsg; /* ¶¨ÒåÒ»¸ö½á¹¹ÌåÓÃÓÚÏûÏ¢¶ÓÁÐ */





/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
//	AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
}


/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
   // MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为500ms */
	uint32_t ulValue;
    
	
    while(1)
    {
		/*
			第一个参数 ulBitsToClearOnEntry的作用（函数执行前）：
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位。
		
		    第二个参数 ulBitsToClearOnExit的作用（函数退出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数退出前清楚所有位。
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量。		
		*/
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0)
			{

          
                 xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_POWER_4 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
				                                    
			}

           if((ulValue & MODE_KEY_1) != 0){

               //switch timer timing and works timing 
               if(gkey_t.key_power == power_on){
                

                  key_long_counter=1;

                 // Buzzer_KeySound();
              
                 }
                  
                  
                }   
               //KEY DOWN FUNCTION   
               if((ulValue & DEC_KEY_2) != 0){

                g_tMsg.add_dec_key_input_flag++;
                Dec_Key_Fun(gkey_t.key_mode);

                Buzzer_KeySound();

//                
//                 {

//                     xTaskNotify(xHandleTaskStart, /* 目标任务 */
//							RUN_DEC_6 ,            /* 设置目标任务事件标志位bit0  */
//							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

//                   }


            }

           if((ulValue & ADD_KEY_3) != 0){

                  g_tMsg.add_dec_key_input_flag ++;
                  Add_Key_Fun(gkey_t.key_mode);

                  Buzzer_KeySound();




            }

      

		
		}
		else
		{
			/* 超时 */
       
        
         if(gkey_t.key_power==power_on){

                 Run_Main_Handler();
         
                 LCD_Timer_Colon_Flicker();

                 LCD_Wind_Run_Icon(0);
                 display_works_timer_timing_fun(g_tMsg.key_mode);
                 
                 mode_long_short_key_fun();

               
                     
                     
              
            }
        
			
		}
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
   BaseType_t xResult;
   const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为500ms */
   static uint8_t sound_flag,power_on_first;
   uint32_t ulValue;
   

    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();

       xResult = xTaskNotifyWait(0x00000000,      
						           0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间 */
        if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
			if((ulValue & RUN_POWER_4 ) != 0)
			{
				//printf("接收到K2按键按下消息, ulNotifiedValue = 0x%08x\r\n", ulValue);
				//printf("receive notice key1_bit0 \n");
				
                   sound_flag =1;
                  recieve_flag++;
              
               

                if(sound_flag ==1){
                   sound_flag++;
                   if(gkey_t.key_power==power_off)
                      gkey_t.key_power=power_on;
                   else{
                      gkey_t.key_power=power_off;

                   }
                   Buzzer_KeySound();
                   power_on_first=1; 
                   
                   
                }
		
			}

            /* 接收到消息，检测那个位被按下 */
			if((ulValue & RUN_MODE_5 ) != 0)
			{
               //switch timer timing and works timing 
               if(gkey_t.key_power == power_on){
                
               	 // gkey_t.key_sound_flag = key_sound;
                  gkey_t.key_mode_times = gkey_t.key_mode_times ^ 0x01;
                  if(gkey_t.key_mode_times == 1){
                      gkey_t.key_mode = mode_set_timer;
                      
                  }
                  else{

                     gkey_t.key_mode = mode_confirm; //如果我只按一次那？，经过一段时间推出，恢复 
                     
                  }
                  
                  Buzzer_KeySound();
                }   
            }

             /* 接收到消息，检测那个位被按下 */
			if((ulValue & RUN_DEC_6 ) != 0)
			{


            }


             /* 接收到消息，检测那个位被按下 */
			if((ulValue & RUN_ADD_7 ) != 0)
			{


            }

			
		}
        else{
    	    

            if(power_on_first == 1 && gkey_t.key_power==power_on){
               power_on_first++; 
               PowerOn_Init();
               LCD_Numbers1234_Init();
               Display_Wind_Icon_Inint();

            }
            else if(gkey_t.key_power==power_off){
              PowerOff_freeFun();


            }
          //  mainboard_process_handler();
          //  vTaskDelay(10);
            if(power_on_first==2 && gkey_t.key_power==power_on){

            // LCD_Timer_Colon_Flicker();

           //  Run_Main_Handler();

           }

         }
    }
}
				
/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{

	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 256,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 256,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{

 
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);
 
   switch(GPIO_Pin){

   case KEY_POWER_Pin:

    if(KEY_POWER_VALUE()==1){

        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
        POWER_KEY_0,      /* 设置目标任务事件标志位bit0  */
        eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        &xHigherPriorityTaskWoken);

        /* Èç¹ûxHigherPriorityTaskWoken = pdTRUE£¬ÄÇÃ´ÍË³öÖÐ¶ÏºóÇÐµ½µ±Ç°×î¸ßÓÅÏÈ¼¶ÈÎÎñÖ´ÐÐ */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


    }
            
   
   break;

   case KEY_MODE_Pin:
      if(KEY_MODE_VALUE() == 1){
        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
               MODE_KEY_1,     /* 设置目标任务事件标志位bit0  */
               eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
               &xHigherPriorityTaskWoken);

        /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


       }
   
   break;


   case KEY_UP_Pin:
    
         xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                ADD_KEY_3,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
       
   break;

   case KEY_DOWN_Pin:

       
        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                DEC_KEY_2,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

   break;


   }



}
/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   

*********************************************************************************/
static void mode_long_short_key_fun(void)
{
    if(KEY_MODE_VALUE() == 1 && key_long_counter < 10){


        key_long_counter++;
        if(key_long_counter > 9){

            g_tMsg.key_mode = mode_set_timer;
            gctl_t.ai_flag = 0; //timer tiiming model
            gkey_t.gTimer_disp_set_timer = 0;       //counter exit timing this "mode_set_timer"
            Buzzer_KeySound();

        }

    }
    else if(KEY_MODE_VALUE() == 0 && key_long_counter >0 && key_long_counter<8){ //short key of function

        key_long_counter=0;
        Buzzer_KeySound();
        g_tMsg.ucMessageID=0;
        if(g_tMsg.key_mode  == disp_works_timing){
            g_tMsg.key_mode  = disp_timer_timing;
            gctl_t.ai_flag = 0; //timer tiiming model
            gkey_t.gTimer_disp_switch_disp_mode = 0;       //counter exit timing this "mode_set_timer"

        }
        else{
            g_tMsg.key_mode  = disp_works_timing;
            gctl_t.ai_flag = 1; //timer tiiming model

        }

    }


}

/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   

*********************************************************************************/
static void display_works_timer_timing_fun(uint8_t sel_item)
{


   static uint8_t disp_timer_ref = 0xff ,disp_works_ref = 0xff;
    switch(sel_item){

    case disp_works_timing :
   
        gctl_t.ai_flag =1;
        if(disp_works_ref !=  g_tMsg.disp_timer_works_switch_flag){

           disp_works_ref =  g_tMsg.disp_timer_works_switch_flag;
           LCD_Number_OneTwo_Humidity();

        }

        Display_Works_Timing();
       
    break;
    
    case disp_timer_timing:
   

        gctl_t.ai_flag =0;
        if(disp_timer_ref != g_tMsg.disp_timer_works_switch_flag){

             disp_timer_ref = g_tMsg.disp_timer_works_switch_flag;

             LCD_Number_OneTwo_Humidity();

        }

        Display_Timer_Timing(gProcess_t.set_timer_timing_hours,gProcess_t.set_timer_timing_minutes);


        if( g_tMsg.set_timer_timing_success == 0 && gkey_t.gTimer_disp_switch_disp_mode > 3){

            g_tMsg.key_mode  = disp_works_timing;
            g_tMsg.disp_timer_works_switch_flag++;

        }


    
    break;


    case mode_set_timer:
    

        Set_Timer_Timing_Lcd_Blink(gProcess_t.set_timer_timing_hours,gProcess_t.set_timer_timing_minutes);
        if(gkey_t.gTimer_disp_set_timer > 3){

            if(gProcess_t.set_timer_timing_hours == 0 && gProcess_t.set_timer_timing_minutes==0){

                g_tMsg.set_timer_timing_success = 0;

                gctl_t.ai_flag = 1;

                g_tMsg.disp_timer_works_switch_flag++;
                g_tMsg.key_mode =disp_works_timing;


                }
                else{
                g_tMsg.set_timer_timing_success = 1;
                gProcess_t.gTimer_timer_Counter =0; //start recoder timer timing is "0",from "0" start

                gctl_t.ai_flag = 0;
                g_tMsg.disp_timer_works_switch_flag++;
                g_tMsg.key_mode = disp_timer_timing;

            }



        }

         break;

        }

}

