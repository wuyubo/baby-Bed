#include "spi-wifi.h"

char Ssid[50];
char Passwd[50];
u16 Local_port;
char *sid;
char *pwd;

//******************************************************************
//��������  Wifi_Init
//���ܣ�    wifi ��ʼ��
//�����������
//����ֵ�� ��
//���ڣ�  2016.03.31
//������	����
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
//��������  Wifi_SetSsid
//���ܣ�    wifi �û���
//�����������
//����ֵ�� ��
//���ڣ�  2016.05.03
//������	����
//******************************************************************
void Wifi_SetSsid(char *_ssid, u8 len)
{
	memset(Ssid, 0, sizeof(Ssid));
	memcpy(Ssid, _ssid, len);
}

//******************************************************************
//��������  Wifi_SetPasswd
//���ܣ�    wifi ����
//�����������
//����ֵ�� ��
//���ڣ�  2016.05.03
//������	����
//******************************************************************
void Wifi_SetPasswd(char *_passwd, u8 len)
{
	memset(Passwd, 0, sizeof(Passwd));
	memcpy(Passwd, _passwd, len);
}

//******************************************************************
//��������  Wifi_Connect
//���ܣ�    wifi ��¼��TCP����
//�����������
//����ֵ�� 0���ɹ�;  1,ʧ��
//���ڣ�  2016.04.06
//������	����
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
//��������  Force_Reset
//���ܣ�    ǿ������
//�������
//����ֵ��  
//���ڣ�  2016.04.06
//����
//******************************************************************
void Wifi_ForceReset(void)
{
	Force_Reset();
}
//******************************************************************
//��������  Wifi_Login
//���ܣ�    wifi ��¼
//�����������
//����ֵ�� ��
//���ڣ�  2016.03.31
//������	����
//******************************************************************
u8 Wifi_Login(char *user, char *passwd)
{
	u8 retry=0;
	Login_Wifi(user, passwd);//login wifi(ssid,passwd)
		
	while(Get_WlanStatus() == 0)//�ȴ�����
	{	
		RIGetWlanStatus();
		if(retry >= 10) {
			printf("WlanStatus�ȴ���ʱ���޷���¼��\r\n");
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
//��������  Wifi_ConnectSocket
//���ܣ�    TCP socket ����
//�����������
//����ֵ�� ��
//���ڣ�  2016.03.31
//������	����
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
//��������  wifi_send
//���ܣ�    wifi��������
//���������buf��Ҫ���͵��ַ����� size���ַ����Ĵ�С
//����ֵ��  ��
//���ڣ�  2016.03.31
//������	����
//******************************************************************
int Wifi_Send(INT8U fd, u8 buf[], u32 size)
{
	get_WlanSem();
	if(fd)
	{
		//��socket��������(socket id, �ַ���, �ַ�������)
		RISocketTCPSend(fd, buf, size);
		put_WlanSem();
		return size;
	}
	put_WlanSem();
	return 0;
}


//******************************************************************
//��������  Wifi_Recv
//���ܣ�    wifi��������
//���������buf�����ջ����� size���������Ĵ�С
//����ֵ��  ��
//���ڣ�  2016.03.31
//������	����
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
//��������  wifi_CloseSocket
//���ܣ�    �ر�TCP socket
//���������buf�����ջ����� size���������Ĵ�С
//����ֵ��  ��
//���ڣ�  2016.03.31
//������	����
//******************************************************************
void Wifi_CloseSocket(void)
{
	RISocketClose(Get_SocketID(SERVERID));//�ر�socket
}

void Wifi_CloseServer(void)
{
	RISocketClose(Get_SocketID(CLIENTID));//�ر�socket
}













