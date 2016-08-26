#include"delay.h"
#include "ucos_ii.h"

void delay_us(unsigned int nus)
{
 	 u16 i=0;  
	 while(nus--)
	 {
			i=20;  
			while(i--) ;    
	 }
}


void delay_ms(unsigned int nms)
{
	OSTimeDly(nms);
}
