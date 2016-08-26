#ifndef _CRY_VOICE_H_
#define _CRY_VOICE_H_

#include "main.h"

#define CRYNOCHANGE DATANOCHANGE

#define	VOICE_GPIO_PX	  _VOICE_GPIO_PX
#define	VOICE_HT_GPIOX  _VOICE_HT_GPIOX

#define CRY_VOICE_PIN 	_CRY_VOICE_PIN

#define CRY_VOICE_TRUE 	RESET
#define CRY_VOICE_FALSE SET

#define CRY_STATUS_TRUE 	1
#define CRY_STATUS_FALSE  0

#define CRY_TEMP_COUNT 10

#define READ_STATAUS    CPIO_IN_BITS(VOICE_HT_GPIOX, CRY_VOICE_PIN)



void Voice_Init(void);
u8 Check_Cry(void);

#endif
