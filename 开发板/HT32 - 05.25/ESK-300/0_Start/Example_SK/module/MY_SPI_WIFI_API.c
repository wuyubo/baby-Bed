#include "MY_SPI_WIFI_API.h"

INT8U WlanStatus=0; 	//WIFI连接成功标志
//volatile INT8U SocketStatus=0; //SOCKET连接成功标志
//volatile INT8U socketID = 0; 		//socket id
static INT8U accept_client_socketID = 0;
static INT8U connect_server_socketID = 0;
//连接wifi网络
INT8U Login_Wifi(char *ssid, char *key) {
	
	INT8U retry=0;
	
	RIGetWlanStatus();
	delay_ms(500);
	if(WlanStatus == 0)//没有连接
	{
		 RISetWlanType(0);//设置S为TA模式
		 RISetSSID((INT8U *)ssid);//设置wifi ssid
		 RISetKey(1, 0, (INT8U *) key);
		 RISetNip(0, (INT8U *)"0", (INT8U *)"0", (INT8U *)"0", (INT8U *)"0");//动态ip
		 RISaveParam();
		 RIResetDevice();//重启 
		 delay_ms(500);
		 delay_ms(500);
		 delay_ms(500);
		 RIWlanJoin();
	}
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	return 1;
}


/*
*************************************************
**<sumary>Create_TCP_Server</sumary>
** <LoPort> 本地端口 </type> 
** <RePort> 远程端口 </type> 
**<return> socketID </return>
**吴玉波  2015.05.06
*************************************************
*/

INT8U Create_TCP_Server(u16 LoPort, u16 RePort)
{
	u8 timeout = 120;
	return AppCreatSocketProc(1, (char *)&timeout, LoPort, RePort);	//create socket(server,timeout,local_port,target_port)
}


/*
*************************************************
**<sumary>获取socketID</sumary>
** <type = "CLIENTID"> accept_client_socketID </type> 
** <type = "SERVERID"> connect_server_socketID </type> 
**<return> socketID </return>
**吴玉波  2015.05.06
*************************************************
*/
INT8U Get_SocketID(u8 type)
{
	switch(type)
	{
		case CLIENTID:
			return accept_client_socketID;
		case SERVERID:
			return connect_server_socketID;
			break;
	}
	return 0;
}
/*Get WlanStatus*/
INT8U Get_WlanStatus(void)
{
	return WlanStatus;
}

/*************************************************************/
/**********************read data from wifi********************/
/*************************************************************/
//******************************************************************
//函数名：  get_WifiStatu
//功能：    获取要接收寄存器的状态
//输入参数：无
//返回值：  接收寄存器的状态
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
INT16U get_WifiStatu(void)
{
	INT16U temp = 0;
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_INT_STTS);		//查询SPI_INT_HOST_STTS
	temp |= SPIReadWriteByte(0xff);			//读寄存器，字节序为小端
	temp |= SPIReadWriteByte(0xff) << 8;
	SPINSS(1);
	return temp;
}

//******************************************************************
//函数名：  get_RecDataLen
//功能：    获取要接收数据的长度
//输入参数：无
//返回值：  数据长度
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
INT16U get_RecDataLen(void)
{
	INT16U temp = 0;
	
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_RX_DAT_LEN);	//查询RX_DAT_LEN                                                                                                                                                                                                             
	temp |= SPIReadWriteByte(0xff);
	temp |= SPIReadWriteByte(0xff) << 8;
	SPINSS(1);
	return temp;
}

//******************************************************************
//函数名：  deal_Cmd
//功能：    处理接收到的命令
//输入参数：rxdesc 已接收到的命令
//返回值：  无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
void deal_Cmd(WM_SPI_RX_DESC* rxdesc)
{
	u8 CodeType;
//	SPI_PRINT("MT:%02X\r\n",*(rxdesc->buff + 8));
	SPI_PRINT("CODE:%02X\r\n",*(rxdesc->buff + 9));						 
	CodeType = *(rxdesc->buff + 9);//Code类型
	switch(CodeType) 
	{
		 case RI_CMD_SKCT://创建Socket
			 if(accept_client_socketID == 0)
			 {
						accept_client_socketID = *(rxdesc->buff + 12);
						break;
			 }
			 if(connect_server_socketID == 0 && accept_client_socketID !=0)
						connect_server_socketID = *(rxdesc->buff + 12);
			 break;
		 case RI_EVENT_TCP_CONN://Socket连接成功
//			 SocketStatus = 1;
			 printf("connect...");
			 break;
		 case RI_CMD_LINK_STATUS://网络连接状态
			 if(*(rxdesc->buff + 12) == 1) //；连接
			 {
					WlanStatus = 1;
					SPI_PRINT("IP is:%d.%d.%d.%d\r\n",*(rxdesc->buff + 13),
					 *(rxdesc->buff + 14),*(rxdesc->buff + 15),*(rxdesc->buff + 16));
					 
					SPI_PRINT("netmask is::%d.%d.%d.%d\r\n",*(rxdesc->buff + 17),
					 *(rxdesc->buff + 18),*(rxdesc->buff + 19),*(rxdesc->buff + 20));
					 
					SPI_PRINT("gateway is::%d.%d.%d.%d\r\n",*(rxdesc->buff + 21),
					 *(rxdesc->buff + 22),*(rxdesc->buff + 23),*(rxdesc->buff + 24));
					 
					SPI_PRINT("DNS1 is::%d.%d.%d.%d\r\n",*(rxdesc->buff + 25),
					 *(rxdesc->buff + 26),*(rxdesc->buff + 27),*(rxdesc->buff + 28));
					 
					SPI_PRINT("DNS2 is::%d.%d.%d.%d\r\n\r\n",*(rxdesc->buff + 29),
					 *(rxdesc->buff + 30),*(rxdesc->buff + 31),*(rxdesc->buff + 32));
			 }
			 SPI_PRINT("satau:%02X\r\n",*(rxdesc->buff + 12));	
			 break;
			case RI_CMD_SKCLOSE://socket 关闭
				SPI_PRINT("Socket Close\r\n");
			  connect_server_socketID = 0;
				break;
		}
}

//******************************************************************
//函数名：  deal_Data
//功能：    处理接收到的数据
//输入参数：rxdesc 已接收到的数据
//返回值：  无
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
void deal_Data(WM_SPI_RX_DESC* rxdesc)
{
	u16 len=0;	
	u32 i;
	u8 cmd[2];
	u8 ctr = cmd[1];
	len = *(rxdesc->buff + 2);
	len = (len << 8) + *(rxdesc->buff + 3);	
	Save_ToBuffer((rxdesc->buff + 8), len);
}

//******************************************************************
//函数名：  read_Data
//功能：    wifi接收数据
//输入参数：buf：接收缓冲区 size：接收缓冲区的大小
//返回值：  成功：1   失败：0
//日期：  2016.03.31
//姓名：	吴玉波
//******************************************************************
u8 read_Data(u32 size)
{
	u32 i;
	INT8U buf[100];
	WM_SPI_RX_DESC tmp;
	WM_SPI_RX_DESC* rxdesc;
	tmp.buff = buf;
	tmp.valid = 1;
	rxdesc = &tmp;
	
	if(rxdesc)
	{
		SPINSS(0);
		SPIReadWriteByte(SPI_CMD_RX_DATA);	//读数据命令
		for(i = 0; i < size; i++)
		{
			*(rxdesc->buff+ i) = SPIReadWriteByte(0xff);
		}
		SPINSS(1);
	}
		 //判断数据类型
	if(*(rxdesc->buff + 0) == 0xAA ) //syn
	{
		if(*(rxdesc->buff + 1)==0x01)    //type == 精简指令数据
		{
			deal_Cmd(rxdesc);
		}
		else
		{
			deal_Data(rxdesc);
		}
		return 1;
	}
	return 0;
}

/*****************************************************************/
/**********************end read data from wifi********************/
/*****************************************************************/

