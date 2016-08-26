#include "risym-mp3.h"
static u8 mp3_state = MUSIC_STATUS_OFF;
static u8 volume_value = 50;
static u8 first;
static void music_open(void);
static void music_pause(void);

//******************************************************************
//函数名：  music_mode
//功能：   	设置模式：SD/USB
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void music_mode(void)
{
		CHANGE_MODE();
}

//******************************************************************
//函数名：  music_close
//功能：   	关闭MP3
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void music_close(void)
{
	if(mp3_state != MUSIC_STATUS_OFF)
	{
		MUSIC_CLOSE;
	}
}

//******************************************************************
//函数名：  music_init
//功能：    MP3模块IO初始化
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
void Music_Init(void)
{
	//输出
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_NEXT_PIN);
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_PREV_PIN);
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_OPEN_PIN);
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_PAUSE_PIN);
	//输入
	GPIO_IN_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, ULTRA_ECHO);
	
	CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_NEXT_PIN, MUSIC_BT_UP);
	CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_PREV_PIN, MUSIC_BT_UP);
	CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_OPEN_PIN, MUSIC_BT_UP);
	CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_PAUSE_PIN, MUSIC_BT_UP);
	
	Mp3_Init_Set();
}

void Mp3_Init_Set(void)
{
	music_close();
//	music_open();
//	delay_ms(1000);
//	music_pause();
	//music_pause();
	first = 1;
	mp3_state = MUSIC_STATUS_OFF;
}
//******************************************************************
//函数名：  music_next
//功能：    下一曲
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void music_next(void)
{
		MUSIC_CONTR(MUSIC_NEXT_PIN);
}

//******************************************************************
//函数名：  music_prev
//功能：    上一曲
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void music_prev(void)
{
		MUSIC_CONTR(MUSIC_PREV_PIN);
}

//******************************************************************
//函数名：  volume_up
//功能：   	 音量+
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void volume_up(void)
{
	VOLUME_CONTR(MUSIC_NEXT_PIN);
}

//******************************************************************
//函数名：  volume_down
//功能：   	 音量-
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void volume_down(void)
{ 
	VOLUME_CONTR(MUSIC_PREV_PIN);
}
//******************************************************************
//函数名：  set_volume
//功能：   	 设置音量
//输入参数： 音量
//返回值：  无
//日期：  2016.04.22
//姓名：	吴玉波
//******************************************************************
static void set_volume(u8 value)
{
	u8 i = volume_value;
	if(value > volume_value)
	{
		for(i = volume_value; i<value; i+=20)
		{
			volume_up();
		}
	}
	else
	{
		for(i = volume_value; i>value; i-=20)
		{
			volume_down();
		}
	}
	volume_value = i;
}
//******************************************************************
//函数名：  music_pause
//功能：   	 暂停/播放
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void music_pause(void)
{
		MUSIC_CONTR(MUSIC_PAUSE_PIN);
}

//******************************************************************
//函数名：  music_open
//功能：   	打开MP3
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void music_open(void)
{ 
		if(mp3_state != MUSIC_STATUS_ON)
		{
			MUSIC_OPEN;
		}
}

//******************************************************************
//函数名：  music_control
//功能：    MP3的中控
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
u8 Music_Control(u8 ctrl)
{
		switch(ctrl)
		{
			case PAUSE:
				music_pause();
				break;
			case NEXT:
				mp3_state = MUSIC_STATUS_ON;
				music_next();
				break;
			case PREV:
				mp3_state = MUSIC_STATUS_ON;
				music_prev();
				break;
			case CLOSE:
				if(mp3_state == MUSIC_STATUS_OFF) break;
				
				music_pause();
//				music_close();
			 mp3_state = MUSIC_STATUS_OFF;
				break;
			case OPEN:
				if(mp3_state == MUSIC_STATUS_ON) break;	
				if(first)
				{
					printf("open...\r\n");
					music_open();
					first = 0;
				}
				else
					music_pause();
				mp3_state = MUSIC_STATUS_ON;
				break;
			case MODE:
				music_mode();
				break;
			default:
				break;
		} 
		if(ctrl > 7)
			set_volume(ctrl);
		return 0;
}

//******************************************************************
//函数名：  Check_Music
//功能：    MP3状态
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
u8 Check_Music()
{
	return mp3_state;
}