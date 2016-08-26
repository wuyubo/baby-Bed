#ifndef __SPI01_H__
#define __SPI01_H__

#include "ht32.h"
#include "ht32_board.h"

#include "wmri.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "delay.h"

/************************(代码移植需改动)************************************/

#define WIFI_CS_Dis()             { GPIO_WriteOutBits(HT_GPIOB,GPIO_PIN_1, SET); }
#define WIFI_CS_EN()              { GPIO_WriteOutBits(HT_GPIOB,GPIO_PIN_1, RESET);}

#define WIFI_SCK_H 		 GPIO_WriteOutBits(HT_GPIOA,GPIO_PIN_5, SET)
#define WIFI_SCK_L		 GPIO_WriteOutBits(HT_GPIOA,GPIO_PIN_5, RESET)
#define WIFI_MOSI_H 	 GPIO_WriteOutBits(HT_GPIOA,GPIO_PIN_7, SET)
#define WIFI_MOSI_L		 GPIO_WriteOutBits(HT_GPIOA,GPIO_PIN_7, RESET)
#define WIFI_MISO 		 GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_6)
#define WIFI_INT       GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_0)
#define WIFI_DELAY {delay_us(1);}

/****************************************************************************/

#define SPI_DEBUG		1
#if SPI_DEBUG
#define SPI_PRINT printf
#else
#define SPI_PRINT(fmt, ...)
#endif
void spiInit(void);
void EXTI_Config(void);
// 下面两个宏不可以同时打开
//#define WM_DEMO_UART	//
#define	WM_DEMO_SPI		//

typedef struct _APP_MSG{
	INT32U type;
	INT32U param;
}APP_MSG;

typedef enum
{
	MSG_KEY = 0x01,
	MSG_LED,	
	MSG_WLAN,
	MSG_TIMER,
	MSG_SPI
}MSG_TYPE;

typedef struct _APP_SYS{
#ifdef WM_DEMO_UART	
	OS_TMR * TimerWlanConnect;
	OS_TMR * TimerWlanRecive;
#endif	
	INT32U WlanStatus;
	INT32U FdTcp;
	INT32U FdUdp;
}APP_SYS;


enum socket_protocol{
    SOCKET_PROTO_TCP,      /* TCP Protocol */
    SOCKET_PROTO_UDP,     /* UDP Protocol */
};

enum socket_cs_mode{
    SOCKET_CS_MODE_CLIENT,    /* Client mode */
    SOCKET_CS_MODE_SERVER,    /* Server mode */
};

typedef struct _WM_SOCKET_INFO{
	enum socket_protocol protocol;
	enum socket_cs_mode cs;
	INT8U hostname[32];
	INT16U remote;
	INT16U local;
}WM_SOCKET_INFO;

typedef struct _WM_SPI_RX_DESC{
    INT32U valid;
    INT8U* buff;
}WM_SPI_RX_DESC;

#define WM_MALLOC		malloc
#define WM_FREE			free

/***********************************************************************
* WLAN
***********************************************************************/
#define APP_WLAN_STA			0
#define APP_WLAN_AP				2

#define APP_ENCRY_OPEN			0
#define APP_ENCRY_WEP64			1
#define APP_ENCRY_WEP128		2
#define APP_ENCRY_WPA_TKIP		3
#define APP_ENCRY_WPA_CCMP		4
#define APP_ENCRY_WPA2_TKIP		5
#define APP_ENCRY_WPA2_CCMP		6

#define APP_KEY_HEX				0
#define APP_KEY_ASCII			1

#define APP_KEY_DHCP_ENABLE		0
#define APP_KEY_DHCP_DISABLE	1

/***********************************************************************
* WLAN 消息ID
***********************************************************************/
#define APP_WLAN_UP				512
#define APP_WLAN_DOWN			513

/***********************************************************************
* TASK PRIORITIES
***********************************************************************/
#define APP_TASK_PRIO			3
#define LED_TASK_PRIO			4

/***********************************************************************
* TASK STACK SIZES
***********************************************************************/
#define APP_TASK_STK_SIZE				1024
#define APP_QUEUE_SIZE					32

#define LED_TASK_STK_SIZE				64
#define LED_QUEUE_SIZE					4

#define APP_TIMER_WSCAN_TIME		(100 * 3)	// 3s
#define APP_TIMER_WSCAN_ID			1001

#define APP_TIMER_WRECIVE_TIME		50			// 500ms
#define APP_TIMER_WRECIVE_ID		1002

#define SPI_RX_DESC_NUM			32

#define SPI_REG_TIMEOUT			200
#define SPI_TIMEOUT				20		// 200mS

#define SPI_REG_INT_STTS		0x06
#define SPI_REG_RX_DAT_LEN		0x02
#define SPI_REG_TX_BUFF_AVAIL	0x03
#define SPI_CMD_RX_DATA			0x10
#define SPI_CMD_TX_CMD			0x91
#define SPI_CMD_TX_DATA			0x90


void SPIRxData(void);
void WIFI_EXTI_Config(void);
void SPIFreeRxBuff(WM_SPI_RX_DESC* desc);
INT8U SPITxCmd(INT8U *TXBuf, INT16U CmdLen);
INT8U SPITxData(INT8U *TXBuf, INT16U DataLen);
void SPINSS(INT8U status);
WM_SPI_RX_DESC* SPIGetRxBuff(INT32U size);
INT8U SPIReadWriteByte(INT8U TxData);

#endif	   
