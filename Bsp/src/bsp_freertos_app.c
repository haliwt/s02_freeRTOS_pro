#include "bsp.h"
#include "queue.h"

uint8_t gTimer_counter;
uint8_t key_counter;
uint8_t receive_key_conunter;
#if 0
/*
**********************************************************************************************************
											宏定义
**********************************************************************************************************
*/
#define BIT_0	(1 << 0)
#define BIT_1	(1 << 1)

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
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
   // vTaskStartScheduler();
}


/*
*********************************************************************************************************
*	函 数 名: vTaskTaskUserIF
*	功能说明: 接口消息处理。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
	

    while(1)
    {
		ucKeyCode = bsp_GetKey();
		
		if (ucKeyCode != 0)
		{
			switch (ucKeyCode)
			{
				/* K1键按下 打印任务执行情况 */
				case 1:			 
					//printf("=================================================\r\n");
					//printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
					//vTaskList((char *)&pcWriteBuffer);
					//printf("%s\r\n", pcWriteBuffer);
				
					printf("KEY1_down\n");
					//vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					//printf("%s\r\n", pcWriteBuffer);
					break;
				
				/* K2键按下，直接发送事件标志设置给任务vTaskMsgPro，置位bit0 */
				case 2:
					printf("K2bit0\r\n");
					xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
								BIT_0,             /* 设置目标任务事件标志位bit0  */
								eSetBits);         /* 将目标任务的事件标志位与BIT_0进行或操作， 
				                                      将结果赋值给事件标志位。*/
					break;
				
				/*  K3键按下，直接发送事件标志设置给任务vTaskMsgPro，置位bit1 */
				case 3:
					printf("K3bit1\r\n");
					xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
								BIT_1,             /* 设置目标任务事件标志位bit0  */
								eSetBits);         /* 将目标任务的事件标志位与BIT_0进行或操作， 
				                                      将结果赋值给事件标志位。*/
				
				/* 其他的键值不处理 */
				default:                     
					break;
			}
		}
		
		vTaskDelay(20);
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: LED闪烁
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 200;

	/* 获取当前的系统时间 */
    xLastWakeTime = xTaskGetTickCount();
	
    while(1)
    {
       //	bsp_LedToggle(2);
		//bsp_LedToggle(3);
		
		/* vTaskDelayUntil是绝对延迟，vTaskDelay是相对延迟。*/
      //  vTaskDelayUntil(&xLastWakeTime, xFrequency);
         LED_ON();
			HAL_Delay(500);
			LED_OFF();
			HAL_Delay(500);
       vTaskDelay(300);
    }
    
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
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500); /* 设置最大等待时间为500ms */
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
			if((ulValue & BIT_0) != 0)
			{
				//printf("接收到K2按键按下消息, ulNotifiedValue = 0x%08x\r\n", ulValue);
				printf("receive notice key1_bit0 \n");
		
			}

			if((ulValue & BIT_1) != 0)
			{
				//printf("接收到K下消息ulNotifiedValue = 0x%08x\r\n", ulValue);
				printf("receive notice key1_bit1 \n");
			}
		}
		else
		{
			/* 超时 */
            LED_ON();
			HAL_Delay(10);
			LED_OFF();
			HAL_Delay(10);
			
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
    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();
	   if(KEY1_VALUE()==0 || KEY_UP_VALUE() ==1){
		   key1_pressed_counter++;
			 if( key1_pressed_counter> 3)
				  key1_pressed_counter=1;
				
		}
        vTaskDelay(10);
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
    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 256,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级*/
                 &xHandleTaskUserIF );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务名    */
                 256,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先级*/
                 &xHandleTaskLED ); /* 任务句柄  */
	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 256,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 256,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 4,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}




#endif 

#if 1
/***********************************************************************************************************
											函数声明
**********************************************************************************************************/
//static void vTaskTaskUserIF(void *pvParameters);
//static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);
static void AppObjCreate (void);

/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
//static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;

typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t usData[2];
	uint8_t ulData[2];
}MSG_T;

MSG_T   g_tMsg; /* 定义一个结构体用于消息队列 */


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
	AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    //vTaskStartScheduler();
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTaskUserIF
*	功能说明: 接口消息处理。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
//static void vTaskTaskUserIF(void *pvParameters)
//{
//	
//
//    while(1)
//    {
//		 LED_ON();
//			HAL_Delay(1000);
//			LED_OFF();
//			HAL_Delay(1000);
//		
//		vTaskDelay(300);
//	}
//}

/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
//static void vTaskLED(void *pvParameters)
//{
//    uint8_t ucQueueMsgValue;
//	BaseType_t xResult;
//	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
//	MSG_T *ptMsg;
//    while(1)
//    {
//         /* 按键扫描 */
//	
//     
//       	 
//        xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
//		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
//		                        xMaxBlockTime);/* 设置阻塞时间 */
//		
//		
//		if(xResult == pdPASS)
//		{
//			/* 成功接收，并通过串口将数据打印出来 */
//		//	printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
//		//	printf("接收到消息队列数据ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
//		//	printf("接收到消息队列数据ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
//		//    printf("receive_message_2 is success \n");
//
////            ptMsg->ucMessageID++;
////        xQueueSend(xQueue1,                  /* 消息队列句柄 */
////                     (void *) &ptMsg,        /* 发送结构体指针变量ptMsg的地址 */
////                            0);        /* 发送失败，即使等待了10个时钟节拍 */
//        receive_key_conunter++;
//      //  HAL_UART_Receive(&huart1,&ptMsg->ucMessageID,1,0xffff);
//      LED_ON();
//			HAL_Delay(50);
//			LED_OFF();
//			HAL_Delay(50);
//            LED_ON();
//            HAL_Delay(50);
//            LED_OFF();
//			HAL_Delay(50);
//			LED_OFF();
//			HAL_Delay(50);
//            LED_ON();
//
//            HAL_Delay(50);
//			LED_OFF();
//           
//		}
////		else
////		{
////			/* 超时 */
////			LED_ON();
////			HAL_Delay(10);
////			LED_OFF();
////			HAL_Delay(10);
////			
////		}
//    }
//}

/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue1)
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
     uint8_t ucQueueMsgValue;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
	MSG_T *ptMsg;
    while(1)
    {
         /* 按键扫描 */
	
     
       	 
        xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        xMaxBlockTime);/* 设置阻塞时间 */
		
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
		//	printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
		//	printf("接收到消息队列数据ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
		//	printf("接收到消息队列数据ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
		//    printf("receive_message_2 is success \n");

//            ptMsg->ucMessageID++;
//        xQueueSend(xQueue1,                  /* 消息队列句柄 */
//                     (void *) &ptMsg,        /* 发送结构体指针变量ptMsg的地址 */
//                            0);        /* 发送失败，即使等待了10个时钟节拍 */
        receive_key_conunter++;
       
      LED_ON();
			HAL_Delay(50);
			LED_OFF();
			HAL_Delay(50);
            LED_ON();
            HAL_Delay(50);
            LED_OFF();
			HAL_Delay(50);
			LED_OFF();
			HAL_Delay(50);
            LED_ON();

            HAL_Delay(50);
			LED_OFF();
           
		}
//		else
//		{
//			/* 超时 */
//			LED_ON();
//			HAL_Delay(10);
//			LED_OFF();
//			HAL_Delay(10);
//			
//		}
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
    MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500); /* 设置最大等待时间为200ms */
    uint8_t key_value;
    while(1)
     {

    
        /* 超时 */
			LED_ON();
			HAL_Delay(300);
			LED_OFF();
			HAL_Delay(300);


        vTaskDelay(300);

       
        
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
//    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
//                 "vTaskUserIF",     	/* 任务名    */
//                 512,               	/* 任务栈大小，单位word，也就是4字节 */
//                 NULL,              	/* 任务参数  */
//                 1,                 	/* 任务优先级*/
//                 &xHandleTaskUserIF );  /* 任务句柄  */
	
//	
//	xTaskCreate( vTaskLED,    		/* 任务函数  */
//                 "vTaskLED",  		/* 任务名    */
//                 256,         		/* stack大小，单位word，也就是4字节 */
//                 NULL,        		/* 任务参数  */
//                 1,           		/* 任务优先级*/
//                 &xHandleTaskLED ); /* 任务句柄  */
	
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

/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通信机制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppObjCreate (void)
{
	/* 创建10个uint8_t型消息队列 */
	xQueue1 = xQueueCreate(1, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
	
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
	xQueue2 = xQueueCreate(3, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
}
#endif 


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{

   MSG_T   *ptMsg;
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);


  if(GPIO_Pin==KEY1_Pin){


    if(KEY1_VALUE()  ==1){

  
    key_counter++;
    ptMsg->ucMessageID=1;
	ptMsg->ulData[0]++;
	ptMsg->usData[0]++;

     /* 向消息队列发数据 */
	xQueueSendFromISR(xQueue2,
				      (void *)&ptMsg,
				       &xHigherPriorityTaskWoken);

	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    }

  }
    
  
    
  

}


//void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
//{
//  /* EXTI line interrupt detected */
//  if (__HAL_GPIO_EXTI_GET_RISING_IT(GPIO_Pin) != 0x00u)
//  {
//    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);
//    HAL_GPIO_EXTI_Rising_Callback(GPIO_Pin);
//  }
//
//  if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_Pin) != 0x00u)
//  {
//    __HAL_GPIO_EXTI_CLEAR_FALLING_IT(GPIO_Pin);
//    HAL_GPIO_EXTI_Falling_Callback(GPIO_Pin);
//  }
//}


