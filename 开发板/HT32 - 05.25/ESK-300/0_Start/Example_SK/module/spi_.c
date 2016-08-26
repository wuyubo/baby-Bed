#include "spi.h"


WM_SPI_RX_DESC gsSPIRxDesc[SPI_RX_DESC_NUM];
//******************************************************************
//��������  spiGpioInit
//���ܣ�    wifiģ��IO��ʼ��
//�����������
//����ֵ��  ��
//******************************************************************
/*
void spiGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1  | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);   
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;		 //SCK MISO MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  		//CS
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		 //CS
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //����������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //�������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		//INT
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //����������
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	  //��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
*/
//******************************************************************
//��������  spiCfg
//���ܣ�    wifiģ��SPIģʽ��ʼ��
//���������SPIx��wifiģ��ʹ�õ�SPI�ӿ�  prescaler��SPI��Ƶֵ
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	������
//******************************************************************
/*void spiCfg(uint16_t prescaler)
{
		SPI_InitTypeDef  SPI_InitStructure;
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	      //˫��ģʽ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							  //SPI��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						  //8bit����
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							      //CLK����ʱΪ�͵�ƽ			 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							  //CLK���ز���
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								  //Ƭѡ���������
    SPI_InitStructure.SPI_BaudRatePrescaler = prescaler;	  //SPIƵ�ʣ�72M/256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					  	  //��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;								  //crc7��stm32spi��Ӳ��ecc
    SPI_Init(SPI1, &SPI_InitStructure);
	  SPI_Cmd(SPI1, ENABLE);

}
*/

//ģ��spi�ܽų�ʼ��(������ֲ��Ķ�)
void spiCfg_moni() {

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |  GPIO_Pin_7;		 //SCK MOSI
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //����������
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //�������
//		GPIO_Init(GPIOA, &GPIO_InitStructure);

//		//MISO
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		
//		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	  //��������
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //��������
//		GPIO_Init(GPIOA, &GPIO_InitStructure);	

//		//CS Ƭѡʹ��
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		 //CS
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //����������
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //�������
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//		//INT ����Ƿ������ݷ���
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //����������
//		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	  //��������
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //��������
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	

//		GPIO_SetBits(GPIOB,GPIO_Pin_1);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_5); 
/*****************************************************************/		

}

//******************************************************************
//��������  spiInit
//���ܣ�    spi��ʼ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	������
//******************************************************************
void spiInit(void)
{
	//spiGpioInit();	
  //spiCfg(SPI_BaudRatePrescaler_256);//
	spiCfg_moni();//ģ��spi��ʼ��
	WIFI_CS_Dis();//��ֹƬѡ
	
}
//******************************************************************
//��������  SPIReadWriteByte
//���ܣ�    SPI��дһ���ֽ� 
//���������TxData:Ҫд����ֽ� 
//����ֵ��  ret:��ȡ�����ֽ�
//ex: ����SPIReadWriteByte(dat); д: dat = SPIReadWriteByte(0xff);
//���ڣ�  2016.03.21
//������	������
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
//���ڣ�  2016.03.21
//������	������
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
//���ڣ�  2016.03.21
//������	������
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
//���ڣ�  2016.03.21
//������	������
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
//    
//    desc->valid = 0;
//	return;
}
//******************************************************************
//��������  SPIRxData
//���ܣ�    SPI��wifiģ���ȡ����
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	������
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
			rxdesc = SPIGetRxBuff(temp);
			if(rxdesc)
			{
				SPINSS(0);
				SPIReadWriteByte(SPI_CMD_RX_DATA);	//����������
				for(i = 0; i < temp; i++)
				{
					*(rxdesc->buff+ i) = SPIReadWriteByte(0xff);
           printf("[%d]=[%x]\r\n", i, *(rxdesc->buff + i));
				}
				SPINSS(1);
				
				//rintf("%s", );
				 printf("\r\nSNY:%02X\r\n",*(rxdesc->buff + 0));
         printf("Type:%02X\r\n",(*(rxdesc->buff + 1)));
       //  printf("Len1:%02X\r\n",Len1);
       //  printf("Len2:%02X\r\n",Len2);				 
       //  printf("L:%04X\r\n",datalen);
         printf("SN:%02X\r\n",*(rxdesc->buff + 4));
         printf("FLG:%02X\r\n",*(rxdesc->buff + 5));
         printf("DA:%02X\r\n",*(rxdesc->buff + 6));
         printf("CHK:%02X\r\n",*(rxdesc->buff + 7));
				 printf("MT:%02X\r\n",*(rxdesc->buff + 8));
         printf("CODE:%02X\r\n",*(rxdesc->buff + 9));
         printf("ERR:%02X\r\n",*(rxdesc->buff + 10));
         printf("EXT:%02X\r\n",*(rxdesc->buff + 11));
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
//******************************************************************
//��������  SPITxCmd
//���ܣ�    ��wifiģ�鷢�;���ָ��
//���������TXBuf�����ͻ���  CmdLen�������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	������
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
//        u8 pData[2]={0};
		retry++;
//        pData[0]=SPI_REG_TX_BUFF_AVAIL;
		SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);		//��ѯTX_BUFF_AVAIL
        
		temp |= SPIReadWriteByte(0xff);					//���Ĵ������ֽ���ΪС��
		temp |= SPIReadWriteByte(0xff) << 8;
        
		delay_ms(5);
        printf("temp is %d\r\n",temp);
		if(retry > SPI_TIMEOUT)
		{
			SPI_PRINT("debug SPI Send CMD TIMEOUT\r\n");
			return 0;
		}
	}
	SPINSS(1);
	if(CmdLen > 0)
	{
		if(CmdLen % 4)
		{
			CmdLen = ((CmdLen + 3) / 4) << 2;
		}
		SPI_PRINT("debug TX_BUFF_AVAIL = %d, cmdlen=%d\r\n", temp, CmdLen);
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
//���ڣ�  2016.03.21
//������	������
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





