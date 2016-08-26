#ifndef _UART1_H_
#define _UART1_H_
#include "ht32.h"
#include "ht32_board.h"


void uart1_init(void);
void send_data(HT_USART_TypeDef* USARTx, u8 buf[], u32 size);
void read_data(HT_USART_TypeDef* USARTx, u8 rbuf[], u32 size);

#endif
