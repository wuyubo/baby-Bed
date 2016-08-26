#ifndef _GY_MCU_90615_H_
#define _GY_MCU_90615_H_

#include "main.h"

#define GET_USARTX     HT_USART1
#define ULTRA_PORT	   _TEM_PORT
#define HT_ULTRA_PORT	 _TEM_HT_PORT

#define ULTRA_TRIG		 _TEM_TRIG
#define ULTRA_ECHO		 _TEM_ECHO

#define TRIG_Send(c)   GPIO_WriteOutBits(HT_ULTRA_PORT, ULTRA_TRIG, c)
#define ECHO_Reci  		 GPIO_ReadInBit(HT_ULTRA_PORT, ULTRA_ECHO)

#define T_CMD_LEN  3
#define T_DATA_LEN 10
#define SEND_CHERK_CMD(cmd) send_data(GET_USARTX, cmd, T_CMD_LEN)
#define GET_CHERK_DATA(buf) read_data(GET_USARTX, buf, T_DATA_LEN)

struct TEMPERATURE
{
	int room_temperature;
	int baby_temperature;
};
//---------------------------------------------------------------------------------------------------------------------------------------------
void Temperature_Init(void);
void Ultra_Ranging(float *p);
float infer(float data);
double check(double tem, double distance);
struct TEMPERATURE Check_Temperature(void);
u8 is_onbed(struct TEMPERATURE *t);

#endif
