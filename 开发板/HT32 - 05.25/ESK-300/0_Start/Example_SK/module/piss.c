#include "piss.h"
static u8 current_state;
//******************************************************************
//函数名：  voice_init
//功能：    尿床IO初始化
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
void Piss_Init(void)
{
	//IO输入
	GPIO_IN_CONFIG(PISS_GPIO_PX, PISS_HT_GPIOX, PISS_PIN);
}

//******************************************************************
//函数名：  check_cry
//功能：    尿床状态检测
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
u8 Check_Piss(void)
{
	u8 get_state;
	FlagStatus status = RESET;
	u8 count = 0;
	u8 true_count = 0;
	
	for(count=0; count<PISS_TEMP_COUNT; count++)
	{
		status = READ_PISS_STATAUS;
		switch(status)
		{
			case PISS_TRUE:
				true_count++;
				break;
			case PISS_FALSE:
				break;
		} 
		delay_ms(5);
  }

	if(true_count >= (PISS_TEMP_COUNT/2)) 
			get_state = PISS_STATUS_TRUE;
	else get_state = PISS_STATUS_FALSE;
	return get_state;
}
