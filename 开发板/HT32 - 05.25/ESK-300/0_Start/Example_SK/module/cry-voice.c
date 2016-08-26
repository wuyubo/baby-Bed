#include "cry-voice.h"
static u8 current_state;
//******************************************************************
//函数名：  voice_init
//功能：    声音模块IO初始化
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
void Voice_Init(void)
{
	//IO输入
	GPIO_IN_CONFIG(VOICE_GPIO_PX, VOICE_HT_GPIOX, CRY_VOICE_PIN);
	current_state = 0;
}

//******************************************************************
//函数名：  Check_Cry
//功能：    声音模块状态检测
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
u8 Check_Cry(void)
{
	u8 get_state;
	FlagStatus status = RESET;
	u8 count = 0;
	u8 true_count = 0;
	
	status = READ_STATAUS;
	for(count=0; count<CRY_TEMP_COUNT; count++)
	{
		switch(status)
		{
			case CRY_VOICE_TRUE:
				true_count++;
				break;
			case CRY_VOICE_FALSE:
				break;
		}
		delay_ms(5);
	}
	if(true_count >= (CRY_TEMP_COUNT/3)) 
			get_state = CRY_STATUS_TRUE;
	else get_state = CRY_STATUS_FALSE;
	
	return get_state;
}
