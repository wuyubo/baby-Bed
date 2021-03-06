#include "ht32.h"
#include "ht32_board.h"
#include "tasks.h"

//******************************************************************
//函数名：  Task_RecvData
//功能：    接收wifi数据的任务
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
void Task_RecvData(void *p_arg)
{
	printf("reading\n");
	while(1)
	{
		OSTimeDly(10);
		if(getReadStatu())
		{
				setReadStatu(0);
			  Wifi_Recv();
				run_led(LED_1, 10);
		}
	}	
}
//******************************************************************
//函数名：  Task_DealData
//功能：    处理数据的任务
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
void Task_DealData(void *p_arg)
{
	u8 ret = 0;
	printf("dealing\r\n");
	do
	{
		Wifi_ForceReset();
		ret =  Wifi_Connect();
	}while(ret);
	while(1)
	{
		run_led(LED_2, 200);
		if(bed_client() == CLIENT_ERR);
	}	
}

//******************************************************************
//函数名：  Task_DealRock
//功能：    摇床任务
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Task_DealRock(void *p_arg)
{
	printf("rocking\r\n");
	while(1)
	{
		if(Check_Rock())
			Rocking();
	}	
}

//******************************************************************
//函数名：  Task_DealCommadData
//功能：   
//输入参数：无
//返回值：  无
//日期：  2016.04.23
//姓名：	吴玉波
//******************************************************************
void Task_DealCommadData(void *p_arg)
{
	while(1)
	{
		printf("dealing\n");
		//dealing();
		delay_ms(100);
	}	
}

//******************************************************************
//函数名：  Task_RecvData
//功能：    接收wifi数据的任务
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
//void Task_DealPics(void *p_arg)
//{
//	u8 a = 0;
//	while(1)
//	{
//		run_led(LED_1, 500);
//		if(a == 0 && SocketStatus !=0)
//		{
////			Test_Camara();
//			a=1;
//		}
//	}	
//}




