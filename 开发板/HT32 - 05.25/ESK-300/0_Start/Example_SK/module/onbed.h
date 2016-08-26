#ifndef _ONBED_H_
#define _ONBED_H_

#include "main.h"

#define	ONBED_GPIO_PX	 _ONBED_GPIO_PX
#define	ONBED_HT_GPIOX  _ONBED_HT_GPIOX

#define ONBED_PIN 	     _ONBED_PIN
#define ONBED_TRUE 	 SET
#define ONBED_FALSE  RESET

#define ONBED_STATUS_TRUE 	 1
#define ONBED_STATUS_FALSE   0

#define ONBED_TEMP_COUNT 15

#define READ_ONBED_STATAUS CPIO_IN_BITS(ONBED_HT_GPIOX, ONBED_PIN)

void Onbed_Init(void);
u8 Check_Onbed(void);

#endif
