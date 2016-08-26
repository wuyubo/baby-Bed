#include "spi.h"


WM_SPI_RX_DESC gsSPIRxDesc[SPI_RX_DESC_NUM];
//******************************************************************
//函数名：  spiGpioInit
//功能：    wifi模块IO初始化
//输入参数：无
//返回值：  无
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
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最高输出速率
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		//INT
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最高输出速率
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	  //浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //上拉输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
*/
//******************************************************************
//函数名：  spiCfg
//功能：    wifi模块SPI模式初始化
//输入参数：SPIx：wifi模块使用的SPI接口  prescaler：SPI分频值
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
//******************************************************************
/*void spiCfg(uint16_t prescaler)
{
		SPI_InitTypeDef  SPI_InitStructure;
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	      //双工模式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							  //SPI主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						  //8bit数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							      //CLK空闲时为低电平			 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							  //CLK下沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								  //片选用软件控制
    SPI_InitStructure.SPI_BaudRatePrescaler = prescaler;	  //SPI频率：72M/256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					  	  //高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;								  //crc7，stm32spi带硬件ecc
    SPI_Init(SPI1, &SPI_InitStructure);
	  SPI_Cmd(SPI1, ENABLE);

}
*/

//模拟spi管脚初始化(代码移植需改动)
void spiCfg_moni() {

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |  GPIO_Pin_7;		 //SCK MOSI
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最高输出速率
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //推挽输出
//		GPIO_Init(GPIOA, &GPIO_InitStructure);

//		//MISO
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		
//		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	  //浮空输入
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //上拉输入
//		GPIO_Init(GPIOA, &GPIO_InitStructure);	

//		//CS 片选使能
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		 //CS
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最高输出速率
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //推挽输出
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//		//INT 检测是否有数据返回
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最高输出速率
//		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	  //浮空输入
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //上拉输入
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	

//		GPIO_SetBits(GPIOB,GPIO_Pin_1);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_5); 
/*****************************************************************/		

}

//******************************************************************
//函数名：  spiInit
//功能：    spi初始化
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
//******************************************************************
void spiInit(void)
{
	//spiGpioInit();	
  //spiCfg(SPI_BaudRatePrescaler_256);//
	spiCfg_moni();//模拟spi初始化
	WIFI_CS_Dis();//禁止片选
	
}
//******************************************************************
//函数名：  SPIReadWriteByte
//功能：    SPI读写一个字节 
//输入参数：TxData:要写入的字节 
//返回值：  ret:读取到的字节
//ex: 读：SPIReadWriteByte(dat); 写: dat = SPIReadWriteByte(0xff);
//日期：  2016.03.21
//姓名：	林毅星
//******************************************************************
INT8U SPIReadWriteByte(INT8U TxData)
{		
	unsigned char i, temp;
	temp = 0;
	WIFI_DELAY; 	//延时1us	
	for(i=0; i<8; i++) 
	{
		//主机写操作
		if(TxData & 0x80) 		
			WIFI_MOSI_H;
		else 
			WIFI_MOSI_L;
		TxData <<= 1;
		WIFI_SCK_H;
		WIFI_DELAY;
		//主机读操作
		temp <<= 1;
	  if(WIFI_MISO)		
			temp++;
	  WIFI_SCK_L;
		WIFI_DELAY;
	}
	return temp;		
	
}
//******************************************************************
//函数名：  SPINSS
//功能：    SPI片选
//输入参数：status：1、失能    0：使能
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
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
//函数名：  SPIGetRxBuff
//功能：    申请数据内存
//输入参数：size：数据大小
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
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
//函数名：  SPIFreeRxBuff
//功能：    释放内存
//输入参数：desc：释放内存目标
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
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
//函数名：  SPIRxData
//功能：    SPI从wifi模块获取数据
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
//******************************************************************
void SPIRxData(void)
{
	INT16U temp = 0;
	INT16U i;
	WM_SPI_RX_DESC* rxdesc;
  INT8U tempdata;
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_INT_STTS);		//查询SPI_INT_HOST_STTS
	temp |= SPIReadWriteByte(0xff);			//读寄存器，字节序为小端
	temp |= SPIReadWriteByte(0xff) << 8;
	SPINSS(1);
   printf("SPIRxData temp is %d\r\n",temp);
	if((temp != 0xffff) && (temp & 0x01))	//数据或命令已经准备好
	{
		SPINSS(0);
		SPIReadWriteByte(SPI_REG_RX_DAT_LEN);	//查询RX_DAT_LEN                                                                                                                                                                                                             
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
				SPIReadWriteByte(SPI_CMD_RX_DATA);	//读数据命令
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
            SPIReadWriteByte(SPI_CMD_RX_DATA);	//读数据命令
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
//函数名：  SPITxCmd
//功能：    往wifi模块发送精简指令
//输入参数：TXBuf：发送缓存  CmdLen：命令长度
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
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
		SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);		//查询TX_BUFF_AVAIL
        
		temp |= SPIReadWriteByte(0xff);					//读寄存器，字节序为小端
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
		SPIReadWriteByte(SPI_CMD_TX_CMD);	//写命令命令
		for(i = 0; i < CmdLen; i ++)
		{
			SPIReadWriteByte(*(TXBuf + i));
		}
		SPINSS(1);
	}

	return 1;	
}

//******************************************************************
//函数名：  SPITxData
//功能：    通过SPI发送数据到wifi模块
//输入参数：TXBuf：发送缓存  DataLen：数据长度
//返回值：  无
//日期：  2016.03.21
//姓名：	林毅星
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
		SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);	//查询TX_BUFF_AVAIL
		temp |= SPIReadWriteByte(0xff);				//读寄存器，字节序为小端
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
		SPIReadWriteByte(SPI_CMD_TX_DATA);	//写数据命令
		for(i = 0; i < DataLen; i ++)
		{
			SPIReadWriteByte(*(TXBuf + i));
		}
		SPINSS(1);
	}	
    
	return 1;
}





