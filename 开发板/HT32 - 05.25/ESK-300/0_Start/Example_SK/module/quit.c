#include "quilt.h"
static u8 current_state;
//******************************************************************
//函数名：  voice_init
//功能：    踢被子模块IO初始化
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
void Quilt_Init(void)
{
	//IO输入
	GPIO_IN_CONFIG(QUILT_GPIO_PX, QUILT_HT_GPIOX, QUILT_PIN);
	current_state = 0;
}

//******************************************************************
//函数名：  check_cry
//功能：    踢被子模块状态检测
//输入参数：无
//返回值：  无
//日期：  2016.04.11
//姓名：	吴玉波
//******************************************************************
u8 Check_Quilt(void)
{
	u8 get_state;
	FlagStatus status = RESET;
	u8 count = 0;
	u8 true_count = 0;

	for(count=0; count<QUILT_TEMP_COUNT; count++)
	{
		status = READ_QUILT_STATAUS;
		switch(status)
		{
			case QUILT_TRUE:
				true_count++;
				break;
			case QUILT_FALSE:
				break;
		}
		delay_ms(5);
	}
	
	if(true_count >= (QUILT_TEMP_COUNT/2)) 
			get_state = QUILT_STATUS_TRUE;
	else get_state = QUILT_STATUS_FALSE;
	return get_state;
}
