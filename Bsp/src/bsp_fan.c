#include "bsp_fan.h"
#include "bsp.h"



void Fan_Run(void)
{
    
   FAN_CCW_SetHigh(); //brake
   FAN_COM_SetLow(); //PA6
	 

}

 
void Fan_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    FAN_COM_SetLow(); //PA6
  
    
}

