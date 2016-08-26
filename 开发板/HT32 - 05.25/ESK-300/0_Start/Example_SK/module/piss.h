#ifndef _PISS_H_
#define _PISS_H_

#include "main.h"

#define	PISS_GPIO_PX	 _PISS_GPIO_PX
#define	PISS_HT_GPIOX  _PISS_HT_GPIOX

#define PISS_PIN 	     _PISS_PIN
#define PISS_TRUE 	SET
#define PISS_FALSE  RESET

#define PISS_STATUS_TRUE 	 1
#define PISS_STATUS_FALSE  0

#define PISS_TEMP_COUNT 10

#define READ_PISS_STATAUS CPIO_IN_BITS(PISS_HT_GPIOX, PISS_PIN)

void Piss_Init(void);
u8 Check_Piss(void);

#endif
