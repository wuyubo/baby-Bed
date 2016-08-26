#ifndef _QUILT_H_
#define _QUILT_H_

#include "main.h"

#define	QUILT_GPIO_PX	  _QUILT_GPIO_PX
#define	QUILT_HT_GPIOX  _QUILT_HT_GPIOX

#define QUILT_PIN 	    _QUILT_PIN

#define QUILT_TRUE 	RESET
#define QUILT_FALSE SET

#define QUILT_STATUS_TRUE 	 1
#define QUILT_STATUS_FALSE  0

#define QUILT_TEMP_COUNT 10

#define READ_QUILT_STATAUS CPIO_IN_BITS(QUILT_HT_GPIOX, QUILT_PIN)

void Quilt_Init(void);
u8 Check_Quilt(void);

#endif
