#ifndef _SPI_WIFI_H_
#define _SPI_WIFI_H_
#include "main.h"
#include "MY_SPI_WIFI_API.h"

#define SOCKET_ERR 101
#define WIFI_ERR 103
typedef enum modules
{
	GY_TEMPATE = '0',					/*'0' 温度传感器 */
	RISYM_MP3,								/*'1' MP3 */
	CRY_VOICE,								/*'2' 哭声 */
	FAN_CONTR,								/*'3' 风扇控制 */
	PISS_CONTR,								/*'4' 是否尿床*/
	ROCK_CONTR,								/*'5' 是否尿床*/
}MOD_CONTR;

#define WIFI_USARTX HT_USART0

void Wifi_Init(void);
void Wifi_SetSsid(char *_ssid, u8 len);
void Wifi_SetPasswd(char *_passwd, u8 len);
u8 Wifi_Connect(void);
u8 Wifi_Login(char *user, char *passwd);
u8 Wifi_ConnectSocket(char *sip, int sport);
u8 Wifi_TCP_Server(u16 port);
int Wifi_Send(INT8U fd, u8 buf[], u32 size);
void Wifi_Recv(void);
void Wifi_CloseSocket(void);
void Wifi_ForceReset(void);
u8 check_SocketID(void);


#endif
