#ifndef _OV7670_H
#define _OV7670_H
#include "main.h"

#define OV7670_WRST(c)	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_0, c)		//写指针复位
#define OV7670_RCK(c)		GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_1, c)		//读数据时钟
#define OV7670_RRST(c)	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_4, c)  		//读指针复位
#define OV7670_CS(c)		GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_11, c) 		//片选信号(OE)
#define OV7670_WREN(c)	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_12, c)		//写入FIFO使能
#define OV7670_VSYNC  	GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_15)		//同步信号检测IO
															  					 
#define OV7670_DATA   HT_GPIOB->DINR&0x00FF  					//数据输入端口

u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void Test_Camara(void);

#endif





















