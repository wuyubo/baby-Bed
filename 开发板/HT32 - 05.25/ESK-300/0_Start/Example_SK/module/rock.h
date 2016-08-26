#ifndef _ROCK_H_
#define _ROCK_H_

#include "main.h"

#define	ROCK_GPIO_PX	 _ROCK_GPIO_PX
#define	ROCK_HT_GPIOX  _ROCK_HT_GPIOX

#define ROCK_PIN_SWI 			 _ROCK_PIN_SWI			/*开关IO*/
#define ROCK_PIN_L0 			 _ROCK_PIN_L0      /*位置0IO*/
#define ROCK_PIN_L1 			 _ROCK_PIN_L1     /*位置1IO*/

#define ROCK_TRUE 	SET 
#define ROCK_FALSE  RESET

#define ROCK_STATUS_TRUE 	 1 
#define ROCK_STATUS_FALSE  0

#define ROCK_ON_V	 RESET
#define ROCK_OFF_V SET

#define ROCK_ON_CTR	 1
#define ROCK_OFF_CTR 0
#define ROCK_UP_CTR	 '3'
#define ROCK_DO_CTR  '4'

#define READ_ROCK_LOCATION0 CPIO_IN_BITS(ROCK_HT_GPIOX, ROCK_PIN_L0)
#define READ_ROCK_LOCATION1 CPIO_IN_BITS(ROCK_HT_GPIOX, ROCK_PIN_L1)
#define ROCK_ON   				  CPIO_OUT_BITS(ROCK_HT_GPIOX, ROCK_PIN_SWI, ROCK_ON_V)
#define ROCK_OFF  				  CPIO_OUT_BITS(ROCK_HT_GPIOX, ROCK_PIN_SWI, ROCK_OFF_V)

void Rock_Init(void);
u8 Check_Rock(void);
void Rocking(void);
void Rock_Stop(void);
void Rock_Start(void);
void Rock_UP(void);
void Rock_Down(void);
void Rock_Control(u8 cmd);

#endif
