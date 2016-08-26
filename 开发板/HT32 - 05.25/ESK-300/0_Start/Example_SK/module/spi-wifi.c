#include "spi-wifi.h"

char Ssid[50];
char Passwd[50];
u16 Local_port;
char *sid;
char *pwd;

//******************************************************************
//函数名：  Wifi_Init
//功能：    wifi 初始化
//输入参数：无
//返回值： 无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
void Wifi_Init(void)
{
	spiInit();

	sid = Ssid;
	pwd = Passwd;
	Wifi_SetSsid("656", sizeof("656"));
	Wifi_SetPasswd("wuwuwuwei", sizeof("wuwuwuwei"));
	Local_port = 8895;
}

//******************************************************************
//函数名：  Wifi_SetSsid
//功能：    wifi 用户名
//输入参数：无
//返回值： 无
//日期：  2016.05.03
//姓名：	吴玉波
//******************************************************************
void Wifi_SetSsid(char *_ssid, u8 len)
{
	memset(Ssid, 0, sizeof(Ssid));
	memcpy(Ssid, _ssid, len);
}

//******************************************************************
//函数名：  Wifi_SetPasswd
//功能：    wifi 密码
//输入参数：无
//返回值： 无
//日期：  2016.05.03
//姓名：	吴玉波
//******************************************************************
void Wifi_SetPasswd(char *_passwd, u8 len)
{
	memset(Passwd, 0, sizeof(Passwd));
	memcpy(Passwd, _passwd, len);
}

//******************************************************************
//函数名：  Wifi_Connect
//功能：    wifi 登录及TCP连接
//输入参数：无
//返回值： 0，成功;  1,失败
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
u8 Wifi_Connect(void)
{
	u8 ret;
	u8 retry = 0;
	ret = Wifi_Login(sid, pwd);
	//delay_ms(100);
	return ret;
}
//******************************************************************
//函数名：  Force_Reset
//功能：    强制重启
//输入参数
//返回值：  
//日期：  2016.04.06
//吴玉波
//******************************************************************
void Wifi_ForceReset(void)
{
	Force_Reset();
}
//******************************************************************
//函数名：  Wifi_Login
//功能：    wifi 登录
//输入参数：无
//返回值： 无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
u8 Wifi_Login(char *user, char *passwd)
{
	u8 retry=0;
	Login_Wifi(user, passwd);//login wifi(ssid,passwd)
		
	while(Get_WlanStatus() == 0)//等待连接
	{	
		RIGetWlanStatus();
		if(retry >= 10) {
			printf("WlanStatus等待超时。无法登录。\r\n");
			return WIFI_ERR;
		}
		delay_ms(700);
		retry++;
	}
	printf("Login...");
	Wifi_TCP_Server((u16)8899);
	return 0;
}

//******************************************************************
//函数名：  Wifi_ConnectSocket
//功能：    TCP socket 连接
//输入参数：无
//返回值： 无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
u8 Wifi_ConnectSocket(char *sip, int sport)
{
	u8 retry=0;
	if(AppCreatSocketProc(0, sip, Local_port, sport)) return SOCKET_ERR;;	//create socket(client,ip,local_port,target_port)
	
	delay_ms(500);
	while(Get_SocketID(SERVERID) == 0)//socket
	{
		retry++;
		if(retry >= 50) 
		{
			printf("SocketID failed\r\n");
			return SOCKET_ERR;
		}
		delay_ms(100);
	}
	return Get_SocketID(SERVERID);
//	printf("SocketID connect\r\n");
}	

u8 check_SocketID(void)
{
	return Get_SocketID(SERVERID);
}

/*create tcp server*/
u8 Wifi_TCP_Server(u16 port)
{
	u8 ret;
	u8 retry=0;
	ret = Create_TCP_Server((u16)8899, port);
	
	delay_ms(500);
	while(Get_SocketID(CLIENTID) == 0)//socket
	{
		retry++;
		if(retry >= 50) 
		{
			printf("SocketID failed\r\n");
			return SOCKET_ERR;
		}
		delay_ms(100);
	}
	printf("server socket %d", Get_SocketID(CLIENTID));
	return Get_SocketID(CLIENTID);
}

//******************************************************************
//函数名：  wifi_send
//功能：    wifi发送数据
//输入参数：buf：要发送的字符串， size：字符串的大小
//返回值：  无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
int Wifi_Send(INT8U fd, u8 buf[], u32 size)
{
	get_WlanSem();
	if(fd)
	{
		//向socket发送数据(socket id, 字符串, 字符串长度)
		RISocketTCPSend(fd, buf, size);
		put_WlanSem();
		return size;
	}
	put_WlanSem();
	return 0;
}


//******************************************************************
//函数名：  Wifi_Recv
//功能：    wifi接收数据
//输入参数：buf：接收缓冲区 size：缓冲区的大小
//返回值：  无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
void Wifi_Recv(void)
{
	INT16U temp = 0;
	get_WlanSem();
	temp = get_WifiStatu();
	if((temp != 0xffff) && (temp & 0x01))
	{
		temp = get_RecDataLen();
		if(temp > 0)
		{
			if(temp % 4)
			{
				temp = ((temp + 3) / 4) << 2;
			}
			read_Data(temp);
		}
	}
	put_WlanSem();
}

//******************************************************************
//函数名：  wifi_CloseSocket
//功能：    关闭TCP socket
//输入参数：buf：接收缓冲区 size：缓冲区的大小
//返回值：  无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
void Wifi_CloseSocket(void)
{
	RISocketClose(Get_SocketID(SERVERID));//关闭socket
}

void Wifi_CloseServer(void)
{
	RISocketClose(Get_SocketID(CLIENTID));//关闭socket
}













