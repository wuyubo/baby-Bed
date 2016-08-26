#include "spi.h"
#include "uart1.h"

WM_SPI_RX_DESC gsSPIRxDesc[SPI_RX_DESC_NUM];

//模拟spi管脚初始化(代码移植需改动)
void spiCfg_moni() {
		//SCK
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT1, WIFI_HT_CPIO_PORT1, SCK_PIN);
		// MOSI
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT1, WIFI_HT_CPIO_PORT1, MOSI_PIN);
		//MISO
		GPIO_IN_CONFIG(WIFI_CPIO_PORT1,  WIFI_HT_CPIO_PORT1, MISO_PIN);
		//CS 片选使能
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT1, WIFI_HT_CPIO_PORT1, CS_PIN);
		//INT 检测是否有数据返回
//		GPIO_IN_CONFIG(WIFI_CPIO_PORT2,  WIFI_HT_CPIO_PORT2, INT_PIN);
		//RESET
		GPIO_OUT_CONFIG(WIFI_CPIO_PORT2, WIFI_HT_CPIO_PORT2, RESET_PIN);
		
		CPIO_OUT_BITS(WIFI_HT_CPIO_PORT1, CS_PIN, SET);
		CPIO_OUT_BITS(WIFI_HT_CPIO_PORT2, RESET_PIN, SET);
		CPIO_OUT_BITS(WIFI_HT_CPIO_PORT1, SCK_PIN, RESET);
}

//******************************************************************
//函数名：  spiInit
//功能：    spi初始化
//输入参数：无
//返回值：  无
//******************************************************************
void spiInit(void)
{
	//spiGpioInit();	
  //spiCfg(SPI_BaudRatePrescaler_256);//
	spiCfg_moni();//模拟spi初始化
	WIFI_CS_Dis();//禁止片选
	
}
//******************************************************************
//函数名：  Force_Reset
//功能：    强制重启
//输入参数
//返回值：  
//吴玉波
//******************************************************************
void Force_Reset(void)
{
	CPIO_OUT_BITS(WIFI_HT_CPIO_PORT2, RESET_PIN, RESET);
	delay_ms(200);
	CPIO_OUT_BITS(WIFI_HT_CPIO_PORT2, RESET_PIN, SET);
}


//******************************************************************
//函数名：  SPIReadWriteByte
//功能：    SPI读写一个字节 
//输入参数：TxData:要写入的字节 
//返回值：  ret:读取到的字节
//ex: 读：SPIReadWriteByte(dat); 写: dat = SPIReadWriteByte(0xff);
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
//函数名：  SPIRxData
//功能：    SPI从wifi模块获取数据
//输入参数：无
//返回值：  无
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
			delay_ms(100);
			rxdesc = SPIGetRxBuff(temp);
			if(rxdesc)
			{
				SPINSS(0);
				SPIReadWriteByte(SPI_CMD_RX_DATA);	//读数据命令
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

INT16U GetStatus()
{ 
	INT8U buf[2];
	SPINSS(0);
	SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);		//查询TX_BUFF_AVAIL
	buf[0] = SPIReadWriteByte(0xff);
	buf[1] = SPIReadWriteByte(0xff);
	SPINSS(1);
	return (buf[0]&0x03);
}
//******************************************************************
//函数名：  SPITxCmd
//功能：    往wifi模块发送精简指令
//输入参数：TXBuf：发送缓存  CmdLen：命令长度
//返回值：  无
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
//		SPIReadWriteByte(SPI_REG_TX_BUFF_AVAIL);		//查询TX_BUFF_AVAIL
//		temp |= SPIReadWriteByte(0xff);					//读寄存器，字节序为小端
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




