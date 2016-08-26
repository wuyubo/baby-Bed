#ifndef _MY_SPI_WIFI_API_H_
#define _MY_SPI_WIFI_API_H_
#include "main.h"
#include "wmri.h"
#include "delay.h"
#include "spi.h"
#define CLIENTID 0
#define SERVERID 1
//extern unsigned char WlanStatus;
//extern volatile INT8U  socketID;
//extern volatile INT8U SocketStatus; 

unsigned char  Login_Wifi(char *ssid, char* key);
INT8U Create_TCP_Server(u16 LoPort, u16 RePort);
INT8U Get_SocketID(u8 type);
INT8U Get_WlanStatus(void);
u8 read_Data(u32 size);
u8 read_Data(u32 size);
INT16U get_RecDataLen(void);
INT16U get_WifiStatu(void);

#endif
