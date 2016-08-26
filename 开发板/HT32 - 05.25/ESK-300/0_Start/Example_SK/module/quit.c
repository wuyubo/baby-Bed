#include "quilt.h"
static u8 current_state;
//******************************************************************
//��������  voice_init
//���ܣ�    �߱���ģ��IO��ʼ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.04.11
//������	����
//******************************************************************
void Quilt_Init(void)
{
	//IO����
	GPIO_IN_CONFIG(QUILT_GPIO_PX, QUILT_HT_GPIOX, QUILT_PIN);
	current_state = 0;
}

//******************************************************************
//��������  check_cry
//���ܣ�    �߱���ģ��״̬���
//�����������
//����ֵ��  ��
//���ڣ�  2016.04.11
//������	����
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
