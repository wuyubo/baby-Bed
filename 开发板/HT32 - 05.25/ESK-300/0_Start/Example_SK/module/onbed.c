#include "onbed.h"

//******************************************************************
//函数名：  voice_init
//功能：    尿床IO初始化
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
void Onbed_Init(void)
{
	//IO输入
	GPIO_IN_CONFIG(ONBED_GPIO_PX, ONBED_HT_GPIOX, ONBED_PIN);
}

//******************************************************************
//函数名：  check_cry
//功能：    尿床状态检测
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
u8 Check_Onbed(void)
{
	FlagStatus status = RESET;
	u8 count = 0;
	u8 true_count = 0;
	
	for(count=0; count<ONBED_TEMP_COUNT; count++)
	{
		status = READ_ONBED_STATAUS;
		switch(status)
		{
			case ONBED_TRUE:
				
				break;
			case ONBED_FALSE:
				true_count++;
				break;
		} 
		delay_ms(5);
  }
	if(true_count >= (ONBED_TEMP_COUNT/2)) 
			return ONBED_STATUS_FALSE;
	return ONBED_STATUS_TRUE;
}
