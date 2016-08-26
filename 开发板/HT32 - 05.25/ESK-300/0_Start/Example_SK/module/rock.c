#include "rock.h"

static u8 rock_state;
static int change_time;
void set_Rock(u32 value);
//******************************************************************
//函数名：  Rock_Init
//功能：    摇床模块IO初始化
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_Init(void)
{
	//IO输入
	GPIO_IN_CONFIG(ROCK_GPIO_PX, ROCK_HT_GPIOX, ROCK_PIN_L0);
	GPIO_IN_CONFIG(ROCK_GPIO_PX, ROCK_HT_GPIOX, ROCK_PIN_L1);
	
	//IO输出
	GPIO_OUT_CONFIG(ROCK_GPIO_PX, ROCK_HT_GPIOX, ROCK_PIN_SWI);
	Rock_Stop();
	set_Rock(4);
}

//******************************************************************
//函数名：  Rock_Status
//功能：    摇床状态
//输入参数：无
//返回值： 	摇床状态
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
u8 Check_Rock(void)
{
	return rock_state;
}

//******************************************************************
//函数名：  Rock_Location
//功能：    磁铁的位置
//输入参数：无
//返回值：  0，左; 1,右; >1;没有到达指定位置
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
static u8 Rock_Location(void)
{
	if(READ_ROCK_LOCATION0 == RESET) return 0;
	if(READ_ROCK_LOCATION1 == RESET) return 1;
	return 5;
}

//******************************************************************
//函数名：  Rock_One
//功能：    摇床
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_One(void)
{
	ROCK_ON;
	delay_ms(800);
	ROCK_OFF;
	delay_ms(change_time);
}

//******************************************************************
//函数名：  Rocking
//功能：    摇床
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rocking(void)
{
	u8 rol = 0;
	Rock_One();
	while(rock_state)
	{
//		if(Rock_Location() == rol)
//		{
			Rock_One();
			rol++;
			rol %= 2;
//		}
		
	}
}

//******************************************************************
//函数名：  Rocking
//功能：    停止摇床
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_Stop(void)
{
	ROCK_OFF;
	rock_state = ROCK_STATUS_FALSE;
}

//******************************************************************
//函数名：  Rock_Start
//功能：    开始摇床
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_Start(void)
{
	rock_state = ROCK_STATUS_TRUE;
}

//******************************************************************
//函数名：  Rock_UP
//功能：    摇床快
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_UP(void)
{
	if(change_time < 3500)
		change_time += 1000;
}

//******************************************************************
//函数名：  Rock_Start
//功能：    摇床慢
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_Down(void)
{
	if(change_time >= 1500)
		change_time -= 1000;
}

void set_Rock(u32 value)
{
	switch(value)
	{
		case 2:
			change_time = 500;
			break;
		case 3:
			change_time = 1000;
			break;
		case 4:
			change_time = 1500;
			break;
		case 5:
			change_time = 2500;
			break;
	}
}
//******************************************************************
//函数名：  Rock_Control
//功能：    摇床控制
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Rock_Control(u8 cmd)
{
	switch(cmd)
	{
		case ROCK_ON_CTR:
			Rock_Start();
			break;
		case ROCK_OFF_CTR:
			Rock_Stop();
			break;
//		case ROCK_UP_CTR:
//			Rock_UP();
//			break;
//		case ROCK_DO_CTR:
//			 Rock_Down();
//			break;
	}
	if(cmd > ROCK_ON_CTR)
	{
		Rock_Start();
		set_Rock(cmd);
	}
}

