#ifndef _MILK_H_
#define _MILK_H_
#include "main.h"

#define	MILK_GPIO_PX		_MILK_GPIO_PX
#define	MILK_HT_GPIOX   _MILK_HT_GPIOX

#define MILK_PIN 				_MILK_PIN

#define MILK_ON_V	  RESET
#define MILK_OFF_V	SET

#define MILK_ON_CTR	 1
#define MILK_OFF_CTR 0

#define MILK_STATUS_ON	 1
#define MILK_STATUS_OFF  0

#define MILK_ON   CPIO_OUT_BITS(MILK_HT_GPIOX, MILK_PIN, MILK_ON_V)
#define MILK_OFF  CPIO_OUT_BITS(MILK_HT_GPIOX, MILK_PIN, MILK_OFF_V)

void Milk_Init(void);
u8 Check_Milk(void);
void Milk_Contorl(u8 cmd);

#endif
