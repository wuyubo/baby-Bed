#include "uart1.h"
#include "ucos_ii.h"
#include "ht32f1655_56_usart.h"
#include <stdio.h>
#include <stdlib.h>

void uart1_init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  USART_InitTypeDef USART_InitStructure;

  /* Enable peripheral clock of AFIO, USART0                                                                */
  CKCUClock.Bit.AFIO   = 1;
  CKCUClock.Bit.USART0 = 1;
	CKCUClock.Bit.USART1 = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3, AFIO_MODE_6);     // TX (PA2), RX (PA3)
	AFIO_GPAConfig(AFIO_PIN_4 | AFIO_PIN_5, AFIO_MODE_6);     // TX (PA4), RX (PA5)

  /* USART0 configuration ----------------------------------------------------------------------------------*/
  /* USART0 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - None parity bit
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

  USART_Init(HT_USART0, &USART_InitStructure);
  USART_TxCmd(HT_USART0, ENABLE);
  USART_RxCmd(HT_USART0, ENABLE);
	
	USART_Init(HT_USART1, &USART_InitStructure);
  USART_TxCmd(HT_USART1, ENABLE);
  USART_RxCmd(HT_USART1, ENABLE);
}

void send_data(HT_USART_TypeDef* USARTx, u8 buf[], u32 size)
{
	u32 i;
	for(i = 0; i<size; i++)
	{
		USART_SendData(USARTx, buf[i]);
		OSTimeDly(1);
	}
	
}

void read_data(HT_USART_TypeDef* USARTx, u8 rbuf[], u32 size)
{
	u32 i;
	for(i = 0; i<size; i++)
	{
		if(USART_GetLineStatus(USARTx, USART_LSR_RFDR) == SET)
		{
			rbuf[i] = USART_ReceiveData(USARTx);
		}
		OSTimeDly(1);
	}
	
}


int fputc(int ch, FILE *f)     //printf ÖØ¶¨Ïò
{  
		USART_SendData(HT_USART0, ch);
    return ch;  
}









