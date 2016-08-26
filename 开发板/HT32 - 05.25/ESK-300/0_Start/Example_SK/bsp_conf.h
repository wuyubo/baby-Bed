#ifndef _BSP_CONF_H_
#define _BSP_CONF_H_

#include "ht32.h"
#include "ht32_board.h"

void NVIC_Configuration(void);
void CKCU_Configuration(void);
void GPIO_Configuration(void);
#if (ENABLE_CKOUT == 1)
void CKOUTConfig(void);
#endif

#endif
