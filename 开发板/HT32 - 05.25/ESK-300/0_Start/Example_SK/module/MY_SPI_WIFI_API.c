#include "MY_SPI_WIFI_API.h"

INT8U WlanStatus=0; 	//WIFI���ӳɹ���־
//volatile INT8U SocketStatus=0; //SOCKET���ӳɹ���־
//volatile INT8U socketID = 0; 		//socket id
static INT8U accept_client_socketID = 0;
static INT8U connect_server_socketID = 0;
//����wifi����
INT8U Login_Wifi(char *ssid, char *key) {
	
	INT8U retry=0;
	
	RIGetWlanStatus();
	delay_ms(500);
	if(WlanStatus == 0)//û������
	{
		 RISetWlanType(0);//����SΪTAģʽ
		 RISetSSID((INT8U *)ssid);//����wifi ssid
		 RISetKey(1, 0, (INT8U *) key);
		 RISetNip(0, (INT8U *)"0", (INT8U *)"0", (INT8U *)"0", (INT8U *)"0");//��̬ip
		 RISaveParam();
		 RIResetDevice();//���� 
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
** <LoPort> ���ض˿� </type> 
** <RePort> Զ�̶˿� </type> 
**<return> socketID </return>
**����  2015.05.06
*************************************************
*/

INT8U Create_TCP_Server(u16 LoPort, u16 RePort)
{
	u8 timeout = 120;
	return AppCreatSocketProc(1, (char *)&timeout, LoPort, RePort);	//create socket(server,timeout,local_port,target_port)
}


/*
*************************************************
**<sumary>��ȡsocketID</sumary>
** <type = "CLIENTID"> accept_client_socketID </type> 
** <type = "SERVERID"> connect_server_socketID </type> 
**<return> socketID </return>
**����  2015.05.06
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
//��������  get_WifiStatu
//���ܣ�    ��ȡҪ���ռĴ�����״̬
//�����������
//����ֵ��  ���ռĴ�����״̬
//���ڣ�  2016.03.31
//������	����
//******************************************************************
INT16U get_WifiStatu(void)
{
	INT16U temp = 0;
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_INT_STTS);		//��ѯSPI_INT_HOST_STTS
	temp |= SPIReadWriteByte(0xff);			//���Ĵ������ֽ���ΪС��
	temp |= SPIReadWriteByte(0xff) << 8;
	SPINSS(1);
	return temp;
}

//******************************************************************
//��������  get_RecDataLen
//���ܣ�    ��ȡҪ�������ݵĳ���
//�����������
//����ֵ��  ���ݳ���
//���ڣ�  2016.03.31
//������	����
//******************************************************************
INT16U get_RecDataLen(void)
{
	INT16U temp = 0;
	
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_RX_DAT_LEN);	//��ѯRX_DAT_LEN                                                                                                                                                                                                             
	temp |= SPIReadWriteByte(0xff);
	temp |= SPIReadWriteByte(0xff) << 8;
	SPINSS(1);
	return temp;
}

//******************************************************************
//��������  deal_Cmd
//���ܣ�    ������յ�������
//���������rxdesc �ѽ��յ�������
//����ֵ��  ��
//���ڣ�  2016.03.31
//������	����
//******************************************************************
void deal_Cmd(WM_SPI_RX_DESC* rxdesc)
{
	u8 CodeType;
//	SPI_PRINT("MT:%02X\r\n",*(rxdesc->buff + 8));
	SPI_PRINT("CODE:%02X\r\n",*(rxdesc->buff + 9));						 
	CodeType = *(rxdesc->buff + 9);//Code����
	switch(CodeType) 
	{
		 case RI_CMD_SKCT://����Socket
			 if(accept_client_socketID == 0)
			 {
						accept_client_socketID = *(rxdesc->buff + 12);
						break;
			 }
			 if(connect_server_socketID == 0 && accept_client_socketID !=0)
						connect_server_socketID = *(rxdesc->buff + 12);
			 break;
		 case RI_EVENT_TCP_CONN://Socket���ӳɹ�
//			 SocketStatus = 1;
			 printf("connect...");
			 break;
		 case RI_CMD_LINK_STATUS://��������״̬
			 if(*(rxdesc->buff + 12) == 1) //������
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
			case RI_CMD_SKCLOSE://socket �ر�
				SPI_PRINT("Socket Close\r\n");
			  connect_server_socketID = 0;
				break;
		}
}

//******************************************************************
//��������  deal_Data
//���ܣ�    ������յ�������
//���������rxdesc �ѽ��յ�������
//����ֵ��  ��
//���ڣ�  2016.03.31
//������	����
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
//��������  read_Data
//���ܣ�    wifi��������
//���������buf�����ջ����� size�����ջ������Ĵ�С
//����ֵ��  �ɹ���1   ʧ�ܣ�0
//���ڣ�  2016.03.31
//������	����
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
		SPIReadWriteByte(SPI_CMD_RX_DATA);	//����������
		for(i = 0; i < size; i++)
		{
			*(rxdesc->buff+ i) = SPIReadWriteByte(0xff);
		}
		SPINSS(1);
	}
		 //�ж���������
	if(*(rxdesc->buff + 0) == 0xAA ) //syn
	{
		if(*(rxdesc->buff + 1)==0x01)    //type == ����ָ������
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

