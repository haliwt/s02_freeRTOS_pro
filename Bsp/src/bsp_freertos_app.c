#include "bsp.h"
#include "queue.h"

uint8_t recieve_flag;
uint8_t receive_key_message;
uint8_t receive_task_start;
uint8_t key_long_counter;
uint16_t power_key_long_counter ;
uint8_t  dc_power_on;


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
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为500ms */
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
						          portMAX_DELAY);  /* 最大允许延迟时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0)
			{
   
                     
                 xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_POWER_4 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
				                                    
			}
            else if((ulValue & MODE_KEY_1) != 0){

               //switch timer timing and works timing 

                xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_MODE_5 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
               
            }   
            else if((ulValue & DEC_KEY_2) != 0){

               
                xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_DEC_6 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
               

               
            }
            else if((ulValue & ADD_KEY_3) != 0){
                  
                  xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_ADD_7 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

              
                    
            }

      

		
		}
		else
		{
			/* 超时 */
//       if(gkey_t.key_power==power_on){
//          Disip_Wifi_Icon_State();
//        }
       /// MainBoard_Self_Inspection_PowerOn_Fun();
        
       //  WIFI_Process_Handler();

             
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
   const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为500ms */
   static uint8_t sound_flag,power_on_first;
   uint32_t ulValue;
   static uint8_t add_flag,dec_flag,power_sound_flag;

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
				gkey_t.power_key_long_counter =1;
            }
            else if((ulValue & RUN_MODE_5 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{

                 if(gkey_t.key_power == power_on ){
                

                  gkey_t.key_mode_long_counter=1;

               
              
                 }
				
            }
            else if((ulValue & RUN_DEC_6 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{

               if(gkey_t.key_power==power_on){
                dec_flag =1;

               
               }
                 
            }
            else if((ulValue & RUN_ADD_7 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{

                if(gkey_t.key_power==power_on){

                   add_flag =1;
                 
               

                

                 }
				
            }
            

            
        }
        else {
            
              
        if(power_sound_flag==0){
          power_sound_flag++;

          buzzer_sound();

        }

          power_long_short_key_fun();
        
          if(gkey_t.power_key_long_counter ==0 || gkey_t.power_key_long_counter==200){
          if(gkey_t.key_power==power_on){
                 bsp_Idle();
                 mode_long_short_key_fun();

                if(add_flag ==1){
                     add_flag ++;
                     Buzzer_KeySound();

                 }
                 else if(dec_flag ==1){
                     dec_flag ++;
                     Buzzer_KeySound();

                 }

                 
                 if(add_flag ==2){
                    add_flag ++;
                    Add_Key_Fun(gkey_t.key_add_dec_mode);

                 }
                 else if(dec_flag ==2){

                       dec_flag ++;
                       Dec_Key_Fun(gkey_t.key_add_dec_mode);
                 }

           }
                

            if(gkey_t.key_power==power_on){
              power_on_run_handler();
              Record_WorksOr_Timer_Timing_DonotDisp_Handler();

            }
            else{
             power_off_run_handler();

            }
            MainBoard_Self_Inspection_PowerOn_Fun();
        
            WIFI_Process_Handler();
            

            
         
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
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
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
        DISABLE_INT();
        if(KEY_POWER_VALUE()==KEY_DOWN){

     

        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
        POWER_KEY_0,      /* 设置目标任务事件标志位bit0  */
        eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        &xHigherPriorityTaskWoken);

        /* Èç¹ûxHigherPriorityTaskWoken = pdTRUE£¬ÄÇÃ´ÍË³öÖÐ¶ÏºóÇÐµ½µ±Ç°×î¸ßÓÅÏÈ¼¶ÈÎÎñÖ´ÐÐ */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


        }
            
       ENABLE_INT();
   break;

   case KEY_MODE_Pin:
      DISABLE_INT();
      if(KEY_MODE_VALUE() == KEY_DOWN){
        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
               MODE_KEY_1,     /* 设置目标任务事件标志位bit0  */
               eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
               &xHigherPriorityTaskWoken);

        /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


       }
      ENABLE_INT();
   
   break;


   case KEY_UP_Pin:
       DISABLE_INT();
       if(KEY_ADD_VALUE() == KEY_DOWN){
         xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                ADD_KEY_3,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
       ENABLE_INT();
   break;

   case KEY_DOWN_Pin:
         DISABLE_INT();
        if(KEY_DEC_VALUE() == KEY_DOWN){
        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                DEC_KEY_2,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        ENABLE_INT();
   break;
    }
}

