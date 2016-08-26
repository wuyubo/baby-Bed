#include "onbed.h"

//******************************************************************
//��������  voice_init
//���ܣ�    ��IO��ʼ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
void Onbed_Init(void)
{
	//IO����
	GPIO_IN_CONFIG(ONBED_GPIO_PX, ONBED_HT_GPIOX, ONBED_PIN);
}

//******************************************************************
//��������  check_cry
//���ܣ�    ��״̬���
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
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
