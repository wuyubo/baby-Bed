#include "spi.h"
#include "uart1.h"

WM_SPI_RX_DESC gsSPIRxDesc[SPI_RX_DESC_NUM];

//ģ��spi�ܽų�ʼ��(������ֲ��Ķ�)
void spiCfg_moni() {
		//SCK
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT1, WIFI_HT_CPIO_PORT1, SCK_PIN);
		// MOSI
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT1, WIFI_HT_CPIO_PORT1, MOSI_PIN);
		//MISO
		GPIO_IN_CONFIG(WIFI_CPIO_PORT1,  WIFI_HT_CPIO_PORT1, MISO_PIN);
		//CS Ƭѡʹ��
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT1, WIFI_HT_CPIO_PORT1, CS_PIN);
		//INT ����Ƿ������ݷ���
//		GPIO_IN_CONFIG(WIFI_CPIO_PORT2,  WIFI_HT_CPIO_PORT2, INT_PIN);
		//RESET
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT2, WIFI_HT_CPIO_PORT2, RESET_PIN);
		
		CPIO_OUT_BITS(WIFI_HT_CPIO_PORT1, CS_PIN, SET);
		CPIO_OUT_BITS(WIFI_HT_CPIO_PORT2, RESET_PIN, SET);
		CPIO_OUT_BITS(WIFI_HT_CPIO_PORT1, SCK_PIN, RESET);
}

//******************************************************************
//��������  spiInit
//���ܣ�    spi��ʼ��
//�����������
//����ֵ��  ��
//******************************************************************
void spiInit(void)
{
	//spiGpioInit();	
  //spiCfg(SPI_BaudRatePrescaler_256);//
	spiCfg_moni();//ģ��spi��ʼ��
	WIFI_CS_Dis();//��ֹƬѡ
	
}
//******************************************************************
//��������  Force_Reset
//���ܣ�    ǿ������
//�������
//����ֵ��  
//����
//******************************************************************
void Force_Reset(void)
{
	CPIO_OUT_BITS(WIFI_HT_CPIO_PORT2, RESET_PIN, RESET);
	delay_ms(200);
	CPIO_OUT_BITS(WIFI_HT_CPIO_PORT2, RESET_PIN, SET);
}


//******************************************************************
//��������  SPIReadWriteByte
//���ܣ�    SPI��дһ���ֽ� 
//���������TxData:Ҫд����ֽ� 
//����ֵ��  ret:��ȡ�����ֽ�
//ex: ����SPIReadWriteByte(dat); д: dat = SPIReadWriteByte(0xff);
//******************************************************************
INT8U SPIReadWriteByte(INT8U TxData)
{		
	unsigned char i, temp;
	temp = 0;
	WIFI_DELAY; 	//��ʱ1us	
	for(i=0; i<8; i++) 
	{
		//����д����
		if(TxData & 0x80) 		
			WIFI_MOSI_H;
		else 
			WIFI_MOSI_L;
		TxData <<= 1;
		WIFI_SCK_H;
		WIFI_DELAY;
		//����������
		temp <<= 1;
	  if(WIFI_MISO)		
			temp++;
	  WIFI_SCK_L;
		WIFI_DELAY;
	}
	return temp;		
	
}
//******************************************************************
//��������  SPINSS
//���ܣ�    SPIƬѡ
//���������status��1��ʧ��    0��ʹ��
//����ֵ��  ��
//******************************************************************
void SPINSS(INT8U status)
{
	if(status)
	{
		WIFI_CS_Dis();
	}
	else
	{
		WIFI_CS_EN();
	}
}
//******************************************************************
//��������  SPIGetRxBuff
//���ܣ�    ���������ڴ�
//���������size�����ݴ�С
//����ֵ��  ��
//******************************************************************
WM_SPI_RX_DESC* SPIGetRxBuff(INT32U size)
{
	INT32U i;

	for(i = 0; i < SPI_RX_DESC_NUM; i++)
	{
		if(0 == gsSPIRxDesc[i].valid)
		{
			gsSPIRxDesc[i].buff = WM_MALLOC(size);
			if(gsSPIRxDesc[i].buff)
			{
				gsSPIRxDesc[i].valid = 1;
				return &gsSPIRxDesc[i];
			}
			else
			{
				SPI_PRINT("debug SPIGetRxBuff malloc err\r\n");
				return NULL;
			}
		}
	}
	SPI_PRINT("debug SPIGetRxBuff gsSPIRxDesc err\r\n");
	return NULL;
}
//******************************************************************
//��������  SPIFreeRxBuff
//���ܣ�    �ͷ��ڴ�
//���������desc���ͷ��ڴ�Ŀ��
//����ֵ��  ��
//******************************************************************
void SPIFreeRxBuff(WM_SPI_RX_DESC* desc)
{
	if(NULL == desc)
	{
		return;
	}
	if(desc->buff)
	{
		WM_FREE(desc->buff);
		desc->buff = NULL;
	}
	desc->valid = 0;
	return;
}
//******************************************************************
//��������  SPIRxData
//���ܣ�    SPI��wifiģ���ȡ����
//�����������
//����ֵ��  ��
//******************************************************************
void SPIRxData(void)
{
	INT16U temp = 0;
	INT16U i;
	WM_SPI_RX_DESC* rxdesc;
  INT8U tempdata;
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_INT_STTS);		//��ѯSPI_INT_HOST_STTS
	temp |= SPIReadWriteByte(0xff);			//���Ĵ������ֽ���ΪС��
	temp |= SPIReadWriteByte(0xff) << 8;
	SPINSS(1);
  printf("SPIRxData temp is %d\r\n",temp);
	if((temp != 0xffff) && (temp & 0x01))	//���ݻ������Ѿ�׼����
	{
		SPINSS(0);
		SPIReadWriteByte(SPI_REG_RX_DAT_LEN);	//��ѯRX_DAT_LEN                                                                                                                                                                                                             
		temp |= SPIReadWriteByte(0xff);
		temp |= SPIReadWriteByte(0xff) << 8;
		SPINSS(1);
		if(temp > 0)
		{
			if(temp % 4)
			{
				temp = ((temp + 3) / 4) << 2;
			}
			SPI_PRINT("debug rx len = %d\r\n",temp);
			delay_ms(100);
			rxdesc = SPIGetRxBuff(temp);
			if(rxdesc)
			{
				SPINSS(0);
				SPIReadWriteByte(SPI_CMD_RX_DATA);	//����������
				for(i = 0; i < temp; i++)
				{
					*(rxdesc->buff+ i) = SPIReadWriteByte(0xff);
           printf("[%x]=[%x]\r\n", i, *(rxdesc->buff + i));	
					delay_ms(100);
				}
				SPINSS(1);
				
//				AppSendMsg(MSG_SPI, (INT32U)rxdesc);
			}
		}
        else
        {
            SPINSS(0);
            SPIReadWriteByte(SPI_CMD_RX_DATA);	//����������
            for(i = 0; i < temp; i++)
            {
                tempdata = SPIReadWriteByte(0xff);
            //	SPI_PRINT("[%d]=[%x]\r\n", i, *(rxdesc->buff + i));
            }
            SPINSS(1);
        }			
	}
}

INT16U GetStatus()
{ 
	INT8U buf[2];
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);		//��ѯTX_BUFF_AVAIL
	buf[0] = SPIReadWriteByte(0xff);
	buf[1] = SPIReadWriteByte(0xff);
	SPINSS(1);
	return (buf[0]&0x03);
}
//******************************************************************
//��������  SPITxCmd
//���ܣ�    ��wifiģ�鷢�;���ָ��
//���������TXBuf�����ͻ���  CmdLen�������
//����ֵ��  ��
//******************************************************************
INT8U SPITxCmd(INT8U *TXBuf, INT16U CmdLen)
{
//	INT8U temp = 0;
	INT16U temp = 0;
	INT16U i;
	INT32U retry = 0;
	
	if(NULL == TXBuf)
	{
		SPI_PRINT("debug SPITxCmd buff == NULL\r\n");
		return 0;
	}
	SPINSS(0);
	while((temp != 0xffff) && (0 == (temp & 0x02)))	
	{
		retry++;
//		SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);		//��ѯTX_BUFF_AVAIL
//		temp |= SPIReadWriteByte(0xff);					//���Ĵ������ֽ���ΪС��
//		temp |= SPIReadWriteByte(0xff) << 8;
		temp = GetStatus();
		delay_ms(5);
   // printf("temp is 0x%x\r\n",temp);
		if(retry > SPI_TIMEOUT)
		{
			SPI_PRINT("TIMEOUT\r\n");
			// system_Reset();
			return SPI_TIMEOUT;
		}	
	}
	
	SPINSS(1);
	if(CmdLen > 0)
	{
		if(CmdLen % 4)
		{
			CmdLen = ((CmdLen + 3) / 4) << 2;
		}
//		SPI_PRINT("debug TX_BUFF_AVAIL = %d, cmdlen=%d\r\n", temp, CmdLen);
		SPINSS(0);
		SPIReadWriteByte(SPI_CMD_TX_CMD);	//д��������
		for(i = 0; i < CmdLen; i ++)
		{
			SPIReadWriteByte(*(TXBuf + i));
		}
		SPINSS(1);
	}

	return 1;	
}
//******************************************************************
//��������  SPITxData
//���ܣ�    ͨ��SPI�������ݵ�wifiģ��
//���������TXBuf�����ͻ���  DataLen�����ݳ���
//����ֵ��  ��
//******************************************************************
INT8U SPITxData(INT8U *TXBuf, INT16U DataLen)
{
	u16 temp = 0;
	u16 i;
	u16 retry=0;
	
	if(NULL == TXBuf)
	{
		return 0;
	}
	SPINSS(0);
	while((temp != 0xffff) && (0 == (temp & 0x01)))	
	{
		retry ++;
		SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);	//��ѯTX_BUFF_AVAIL
		temp |= SPIReadWriteByte(0xff);				//���Ĵ������ֽ���ΪС��
		temp |= SPIReadWriteByte(0xff) << 8;
		delay_ms(1);
		if(retry > SPI_TIMEOUT)
		{
			return 0;
		}
	}
	SPINSS(1);
	if(DataLen > 0)
	{
		if(DataLen % 4)
		{
			DataLen = ((DataLen + 3) / 4) << 2;
		}
		SPINSS(0);
		
		SPIReadWriteByte(SPI_CMD_TX_DATA);	//д��������
		for(i = 0; i < DataLen; i ++)
		{
			SPIReadWriteByte(*(TXBuf + i));
		}
		SPINSS(1);
	}	
    
	return 1;
}




