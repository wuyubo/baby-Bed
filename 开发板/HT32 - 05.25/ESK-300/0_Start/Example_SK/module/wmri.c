/***********************************************************************
*	INCLUDE FILES
***********************************************************************/
#include "wmri.h"
#ifdef WM_DEMO_SPI
/***********************************************************************
*	APPLICATION GLOBALS
***********************************************************************/
static INT8U gsSeqNum = 0;
u8 RI_Send_SN = 0;
//******************************************************************
//��������  StringToIpaddr
//���ܣ�    �ַ���תIP HEX
//���������buf���ַ���  addr�����IP HEX
//����ֵ��  ��
//*****************************************************************
static int StringToIpaddr(const char *buf, INT8U *addr)
{
	int count = 0, rc = 0;
	int in[4];
	char c;

	rc = sscanf(buf, "%u.%u.%u.%u%c", &in[0], &in[1], &in[2], &in[3], &c);
	if (rc != 4 && (rc != 5 || c != '\n'))
	{
		return -1;
	}
	for (count = 0; count < 4; count++) 
	{
		if (in[count] > 255)
		{
			return -1;
		}
		addr[count] = in[count];
	}
	return 0;
}
//******************************************************************
//��������  RICheckSum
//���ܣ�    ����У���
//���������data����Ҫ���������ָ���׵�ַ  len����������ݳ���
//����ֵ��  ��
//******************************************************************
static INT8U RICheckSum(INT8U *data, INT16U len)
{
	INT16U i;
	INT16U sum = 0;
	
	for(i = 0; i < len; i++)
	{
		sum += *data++;
	}
	return sum;
}
//******************************************************************
//��������  RICreateCmdHeader
//���ܣ�    ����ָ�������ͷ��֡
//���������WM_SPI_HDR *header��8�ֽ�֡ͷ  cmd������ָ������  len��ָ���
//����ֵ��  ��
//******************************************************************
static INT32U RICreateCmdHeader(WM_SPI_HDR *header, INT8U cmd, INT16U len)
{	
	if(NULL == header)
	{
		return 1;
	}
	RI_PRINT("DEBUG 0x%x\r\n", cmd);
	header->hdr.sync		= RI_CMD_SYNC;
	header->hdr.type		= RI_CMD;
	header->hdr.length		= Swap16(len + sizeof(WM_RI_CMD_HDR));
	header->hdr.seq_num		= gsSeqNum++;
	header->hdr.flag		= 0;
	header->hdr.dest_addr	= 0;
	header->hdr.chk			= RICheckSum(&header->hdr.type, 6);
	header->cmd.msg_type	= RI_CMD_MT_REQ;
	header->cmd.code		= cmd;
	header->cmd.err			= 0;
	if(len)
	{
		header->cmd.ext		= 1;
	}
	else
	{
		header->cmd.ext		= 0;
	}
	return 0;
}
//******************************************************************
//��������  RICmdQuery
//���ܣ�    ����ָ���
//���������cmd������ָ������
//����ֵ��  ��
//******************************************************************
INT32U RICmdQuery(INT8U cmd)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	RICreateCmdHeader(header, cmd, 0);
	SPITxCmd(buff, sizeof(WM_SPI_HDR));
	return 0;
}
//******************************************************************
//��������  RISaveParam
//���ܣ�    ���������Flash
//�����������
//����ֵ��  ��
//******************************************************************
INT32U RISaveParam(void)
{
	INT8U buff[32];
	WM_SPI_HDR *header;

	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	RICreateCmdHeader(header, RI_CMD_PMTF, 0);
	SPITxCmd(buff, sizeof(WM_SPI_HDR));
	return 0;
}
//******************************************************************
//��������  RIResetDevice
//���ܣ�    ��λģ��
//�����������
//����ֵ��  ��
//******************************************************************
INT32U RIResetDevice(void)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	RICreateCmdHeader(header, RI_CMD_RESET, 0);
	SPITxCmd(buff, sizeof(WM_SPI_HDR));
	return 0;
}
//******************************************************************
//��������  RISetOneShotCfg
//���ܣ�    ����һ������
//���������flag
//����ֵ��  ��
//******************************************************************
INT32U RISetOneShotCfg(INT8U flag)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = flag; 
	len++;
	RICreateCmdHeader(header, RI_CMD_ONESHOT, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
}
//******************************************************************
//��������  RISetWlanType
//���ܣ�    ������������
//���������type����������
//����ֵ��  ��
//******************************************************************
INT32U RISetWlanType(INT32U type)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = type; 
	len++;
	RICreateCmdHeader(header, RI_CMD_WPRT, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
}
//******************************************************************
//��������  RISetSSID
//���ܣ�    ����SSID
//���������ssid��ssid����
//����ֵ��  ��
//******************************************************************
INT32U RISetSSID(INT8U *ssid)
{
	INT8U buff[64];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	len = strlen((char *)ssid);
	buff[offset++] = len; 
	memcpy((void *)&buff[offset++], ssid, len);
	len++;
	RICreateCmdHeader(header, RI_CMD_SSID, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
}
//******************************************************************
//��������  RISetKey
//���ܣ�    ��������
//���������type����Կ����  index����Կ����  key����Կ
//����ֵ��  ��
//******************************************************************
INT32U RISetKey(INT8U type, INT8U index, INT8U *key)
{
	INT8U buff[96];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = type; 
	len++;
	buff[offset++] = index; 
	len++;
	buff[offset++] = strlen((char *)key); 
	len++;
	memcpy((void *)&buff[offset++], key, strlen((char *)key));
	len += strlen((char *)key);
	RICreateCmdHeader(header, RI_CMD_KEY, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
}
//******************************************************************
//��������  RISetEncrypt
//���ܣ�    ���ü��ܷ�ʽ
//���������type����Կ���ܷ�ʽ
//����ֵ��  ��
//******************************************************************
INT32U RISetEncrypt(INT32U type)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = type; 
	len++;
	RICreateCmdHeader(header, RI_CMD_ENCRYPT, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
}
//******************************************************************
//��������  RISetNip
//���ܣ�    ����IP����
//���������dhcp��IP���䷽ʽ
//          ip:   IP��ַ
//          netmast����������
//          gateway������
//          dns��dns��ַ
//����ֵ��  ��
//******************************************************************
INT32U RISetNip(INT8U dhcp, INT8U *ip, INT8U *netmast, INT8U *gateway, INT8U *dns)
{
	INT8U buff[96];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;

	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = dhcp; 
	len++;
//	if(dhcp)
//	{
		StringToIpaddr((char *)ip, &buff[offset]);
		offset += 4; len += 4;
		StringToIpaddr((char *)netmast, &buff[offset]);
		offset += 4; len += 4;
		StringToIpaddr((char *)gateway, &buff[offset]);
		offset += 4; len += 4;
		StringToIpaddr((char *)dns, &buff[offset]);
		offset += 4; len += 4;
//	}
	RICreateCmdHeader(header, RI_CMD_NIP, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;

}
//******************************************************************
//��������  RIWlanJoin
//���ܣ�    ����/�������� STAģʽ�������磻APģʽ��������
//�����������
//����ֵ��  ��
//******************************************************************
INT32U RIWlanJoin(void)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	RICreateCmdHeader(header, RI_CMD_WJOIN, 0);
	SPITxCmd(buff, sizeof(WM_SPI_HDR));
	return 0;
}
//******************************************************************
//��������  RIGetWlanStatus
//���ܣ�    ��ȡ��������״̬
//�����������
//����ֵ��  ��
//******************************************************************
INT32U RIGetWlanStatus(void)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	RICreateCmdHeader(header, RI_CMD_LINK_STATUS, 0);
	SPITxCmd(buff, sizeof(WM_SPI_HDR));
    
	return 0;
}
//******************************************************************
//��������  RISocketCreate
//���ܣ�    ����socket
//���������sk��socket��Ϣ
//����ֵ��  ��
//******************************************************************
INT32U RISocketCreate(WM_SOCKET_INFO *sk)
{
	INT8U buff[64];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = sk->protocol; 
	len++;
	buff[offset++] = sk->cs; 
	len++;
	buff[offset++] = 4; 
	len++;
	StringToIpaddr((char *)sk->hostname, &buff[offset]);
	offset += 4; len += 4;
	*(INT16U *)&buff[offset] = Swap16(sk->remote);
	offset += 2; len += 2;
	*(INT16U *)&buff[offset] = Swap16(sk->local);
	offset += 2; len += 2;
	RICreateCmdHeader(header, RI_CMD_SKCT, len);
	if(SPITxCmd(buff, sizeof(WM_SPI_HDR) + len) == SPI_TIMEOUT) return 1;
	return 0;
}
//******************************************************************
//��������  RISocketClose
//���ܣ�    �ر�socket����
//���������fd��socket��Ϣ
//����ֵ��  ��
//******************************************************************
INT32U RISocketClose(INT32U fd)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = fd; 
	len++;
	RICreateCmdHeader(header, RI_CMD_SKCLOSE, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
} 
//******************************************************************
//��������  RIGetSocketReceiveLen
//���ܣ�    ��ȡ��ǰ�յ������ݳ���
//���������fd��socket��Ϣ
//����ֵ��  ��
//******************************************************************
INT32U RIGetSocketReceiveLen(INT32U fd)
{
	INT8U buff[32];
	WM_SPI_HDR *header;
	INT16U len = 0, offset;
	
	memset((char *)buff, 0, sizeof(buff));
	header = (WM_SPI_HDR *)buff;
	offset = sizeof(WM_SPI_HDR);
	buff[offset++] = fd; 
	len++;
	RICreateCmdHeader(header, RI_CMD_SKSTT, len);
	SPITxCmd(buff, sizeof(WM_SPI_HDR) + len);
	return 0;
}
//******************************************************************
//��������  RICreateDataHeader
//���ܣ�    ����ͷ��֡
//���������WM_SPI_HDR *header��8�ֽ�֡ͷ  fd��socket��Ϣ  len��ָ���  type����������
//����ֵ��  ��
//******************************************************************
INT32U RICreateDataHeader(WM_RI_HDR *header, INT32U fd, INT16U len, INT16U type)
{
	if(NULL == header)
	{
		return 1;
	}
//	RI_PRINT("debug CMD cmd = 0x%x, len = %d\r\n", cmd, len);
	header->sync		= RI_CMD_SYNC;
	header->type		= RI_DATA;
	header->length		= Swap16(len);
	header->seq_num		= gsSeqNum++;
	header->flag		= 0;
	header->dest_addr	= fd & 0x3F;
	if(type)
	{
		header->dest_addr |= 0x40; 
	}
	header->chk = RICheckSum(&header->type, 6);
	return 0;
}
//����Socket 
//mode�� ģʽ��Sever/Client�� 
//SocketHM Serverʱ��ʾ��ʱ����ʱ�䣬Clentдserver��IP��ַ
//LoPort�����ض˿�
//RePort���Զ˶˿�
INT32U AppCreatSocketProc(u8 mode,char *SocketHM,u16 LoPort,u16 RePort)
{
	WM_SOCKET_INFO sk;
	INT32U ret = 0;

	memset(&sk, 0, sizeof(WM_SOCKET_INFO));
	sk.protocol = SOCKET_PROTO_TCP;
	sk.cs =  mode;
  strcpy((char *)sk.hostname, SocketHM);
	sk.remote = RePort;
	sk.local = LoPort;
	ret = RISocketCreate(&sk);

	return ret;
}
//******************************************************************
//��������  RISocketTCPSend
//���ܣ�    socket��������
//���������fd��socket��Ϣ  data�����͵�����  len�����ݳ���
//����ֵ��  ��
//******************************************************************
INT32U RISocketTCPSend(INT32U fd, INT8U *data, INT16U len)
{
	INT8U buff[158];
	WM_RI_HDR *header;

	memset((char *)buff, 0, sizeof(buff));
	header = (WM_RI_HDR *)buff;
	memcpy((void *)&buff[sizeof(WM_RI_HDR)], data, len);
	RICreateDataHeader(header, fd, len, 0);
	SPITxData(buff, sizeof(WM_RI_HDR) + len);
	return 1;
}
//******************************************************************
//��������  TCPSend
//���ܣ�    socket��������
//���������fd��socket��Ϣ  data�����͵�����  len�����ݳ���
//����ֵ��  ��
//******************************************************************
INT32U TCPSend(INT32U fd, INT8U *data, INT16U len)
{
	INT8U buff[64];
	WM_RI_HDR *header;

	memset((char *)buff, 0, sizeof(buff));
	header = (WM_RI_HDR *)buff;
	memcpy((void *)&buff[sizeof(WM_RI_HDR)], data, len);
	RICreateDataHeader(header, fd, len, 0);
	SPITxData(buff, sizeof(WM_RI_HDR) + len);
	return 1;
}
INT RI_Send_Msg(u8 *pData, INT Type , u32 Datalen)
{
	WM_RI_HDR header; 
	memset(&header, 0, sizeof(WM_RI_HDR));

	header.sync = RI_CMD_SYNC;
	header.type = Type;
	header.length = Swap16(Datalen);
	header.seq_num = (RI_Send_SN++==0)? RI_Send_SN++ : (RI_Send_SN-1);
	header.chk = RICheckSum((u8 *)(&header.type),sizeof(WM_RI_HDR)-2);	

    memcpy(pData,&(header.sync),sizeof(WM_RI_HDR));
	return 0;
}
//******************************************************************
//��������  UserLayer_TxHandle_UserDat_ExtADDs
//���ܣ�    �û�������֡����������ַ��Ϣ��֡��pData
//���������pData������ָ��  RemIP���Զ�IP
//          RemPort:�Զ˶˿� LocPort�������˿�
//SocketNum��TCP����Socket�� TCPorUDP��0-TCP 1-UDP
//����ֵ��  ��
//******************************************************************
void UserLayer_TxHandle_UserDat_AddrAdd(u8 *pData,u32 RemIP,u16 RemPort,u16 LocPort)
{    
    pData[0]=(RemIP>>24)&0xFF;
    pData[1]=(RemIP>>16)&0xFF;
    pData[2]=(RemIP>>8)&0xFF;
    pData[3]=(RemIP)&0xFF;
    
    pData[4]=(RemPort>>8)&0xFF;
    pData[5]=(RemPort)&0xFF;
    pData[6]=(LocPort>>8)&0xFF;
    pData[7]=(LocPort)&0xFF;
}
//******************************************************************
//��������  UserLayer_TxHandle_UserDat_ExtADDs
//���ܣ�    �û�������֡����������ַ��Ϣ��֡��pData
//���������pData������ָ��  RemIP���Զ�IP
//          RemPort:�Զ˶˿� LocPort�������˿�
//SocketNum��TCP����Socket�� TCPorUDP��0-TCP 1-UDP
//����ֵ��  ��
//******************************************************************
void UserLayer_TxHandle_UserDat_DataADDs(u8 *pData,u8 *DataAdd,u32 Length)
{
    u16 i;
    for(i=0;i<Length;i++)
    {
        pData[i]=DataAdd[i];
    }
}
#endif
