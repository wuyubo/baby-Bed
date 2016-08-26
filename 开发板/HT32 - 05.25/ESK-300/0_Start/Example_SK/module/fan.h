#ifndef _FAN_H_
#define _FAN_H_
#include "main.h"

#define	FAN_GPIO_PX		_FAN_GPIO_PX
#define	FAN_HT_GPIOX  _FAN_HT_GPIOX

#define FAN_PIN 	    _FAN_PIN

#define FAN_ON_V	RESET
#define FAN_OFF_V	SET

#define FAN_ON_CTR	1
#define FAN_OFF_CTR 0

#define FAN_STATUS_ON	 1
#define FAN_STATUS_OFF 0

#define FAN_ON   CPIO_OUT_BITS(FAN_HT_GPIOX, FAN_PIN, FAN_ON_V)
#define FAN_OFF  CPIO_OUT_BITS(FAN_HT_GPIOX, FAN_PIN, FAN_OFF_V)

void Fan_Init(void);
u8 Check_Fan(void);
void Fan_Contorl(u8 cmd);

#endif
