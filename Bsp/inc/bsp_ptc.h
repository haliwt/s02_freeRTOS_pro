#ifndef __BSP_PTC_H
#define __BSP_PTC_H
#include "main.h"

#define PTC_SetHigh()          do{RELAY_GPIO_Port->BSRR |= RELAY_Pin; }while(0)  //HAL_GPIO_WritePin(RELAY_GPIO_Port,RELAY_Pin,GPIO_PIN_SET)    // output high level
#define PTC_SetLow()           do{RELAY_GPIO_Port->BSRR |=(uint32_t)RELAY_Pin <<16;}while(0)  /// HAL_GPIO_WritePin(RELAY_GPIO_Port,RELAY_Pin,GPIO_PIN_RESET)    // output low level

void Ptc_On(void);

void Ptc_Off(void);



#endif 

