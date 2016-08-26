#ifndef _OV7670_H
#define _OV7670_H
#include "main.h"

#define OV7670_WRST(c)	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_0, c)		//дָ�븴λ
#define OV7670_RCK(c)		GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_1, c)		//������ʱ��
#define OV7670_RRST(c)	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_4, c)  		//��ָ�븴λ
#define OV7670_CS(c)		GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_11, c) 		//Ƭѡ�ź�(OE)
#define OV7670_WREN(c)	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_12, c)		//д��FIFOʹ��
#define OV7670_VSYNC  	GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_15)		//ͬ���źż��IO
															  					 
#define OV7670_DATA   HT_GPIOB->DINR&0x00FF  					//��������˿�

u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void Test_Camara(void);

#endif





















