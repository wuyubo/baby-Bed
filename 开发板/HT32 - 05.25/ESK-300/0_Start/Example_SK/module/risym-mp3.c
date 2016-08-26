#include "risym-mp3.h"
static u8 mp3_state = MUSIC_STATUS_OFF;
static u8 volume_value = 50;
static u8 first;
static void music_open(void);
static void music_pause(void);

//******************************************************************
//��������  music_mode
//���ܣ�   	����ģʽ��SD/USB
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void music_mode(void)
{
		CHANGE_MODE();
}

//******************************************************************
//��������  music_close
//���ܣ�   	�ر�MP3
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void music_close(void)
{
	if(mp3_state != MUSIC_STATUS_OFF)
	{
		MUSIC_CLOSE;
	}
}

//******************************************************************
//��������  music_init
//���ܣ�    MP3ģ��IO��ʼ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
void Music_Init(void)
{
	//���
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_NEXT_PIN);
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_PREV_PIN);
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_OPEN_PIN);
	GPIO_OUT_CONFIG(MUSIC_GPIO_PX, MUSIC_HT_GPX, MUSIC_PAUSE_PIN);
	//����
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
//��������  music_next
//���ܣ�    ��һ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void music_next(void)
{
		MUSIC_CONTR(MUSIC_NEXT_PIN);
}

//******************************************************************
//��������  music_prev
//���ܣ�    ��һ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void music_prev(void)
{
		MUSIC_CONTR(MUSIC_PREV_PIN);
}

//******************************************************************
//��������  volume_up
//���ܣ�   	 ����+
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void volume_up(void)
{
	VOLUME_CONTR(MUSIC_NEXT_PIN);
}

//******************************************************************
//��������  volume_down
//���ܣ�   	 ����-
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void volume_down(void)
{ 
	VOLUME_CONTR(MUSIC_PREV_PIN);
}
//******************************************************************
//��������  set_volume
//���ܣ�   	 ��������
//��������� ����
//����ֵ��  ��
//���ڣ�  2016.04.22
//������	����
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
//��������  music_pause
//���ܣ�   	 ��ͣ/����
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void music_pause(void)
{
		MUSIC_CONTR(MUSIC_PAUSE_PIN);
}

//******************************************************************
//��������  music_open
//���ܣ�   	��MP3
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void music_open(void)
{ 
		if(mp3_state != MUSIC_STATUS_ON)
		{
			MUSIC_OPEN;
		}
}

//******************************************************************
//��������  music_control
//���ܣ�    MP3���п�
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
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
//��������  Check_Music
//���ܣ�    MP3״̬
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
u8 Check_Music()
{
	return mp3_state;
}