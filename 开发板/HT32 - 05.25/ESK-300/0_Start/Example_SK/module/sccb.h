#ifndef __SCCB_H
#define __SCCB_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ο�������guanfu_wang���롣
//ALIENTEK MiniSTM32������
//OV7670 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/31
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////

#define SCCB_SDA_IN()  {GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_5, GPIO_DIR_IN); \
													GPIO_InputConfig(HT_GPIOC, GPIO_PIN_5, ENABLE);}
#define SCCB_SDA_OUT() {GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_5, GPIO_DIR_OUT);}

//IO��������	 
#define SCCB_SCL(c)    		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_4, c)	 	//SCL
#define SCCB_SDA(c)    		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_5, c) 		//SDA	 

#define SCCB_READ_SDA    	GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_5)  		//����SDA    
#define SCCB_ID   			0X42  			//OV7670��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













