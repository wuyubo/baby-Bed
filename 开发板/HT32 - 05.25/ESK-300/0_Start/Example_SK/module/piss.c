#include "piss.h"
static u8 current_state;
//******************************************************************
//��������  voice_init
//���ܣ�    ��IO��ʼ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
void Piss_Init(void)
{
	//IO����
	GPIO_IN_CONFIG(PISS_GPIO_PX, PISS_HT_GPIOX, PISS_PIN);
}

//******************************************************************
//��������  check_cry
//���ܣ�    ��״̬���
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
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
