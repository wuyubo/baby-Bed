#include "gy-mcu90615.h"
struct TEMPERATURE current_temp;
//******************************************************************
//��������  Ultran_Init
//���ܣ�     ������ģ���ʼ��
//��������� ��
//����ֵ��  ��
//���ڣ�  2016.04.06
//������	����
//******************************************************************
static void Ultran_Init(void)
{
	//trig���
	GPIO_OUT_CONFIG(ULTRA_PORT, HT_ULTRA_PORT, ULTRA_TRIG);
	//echo ����
	GPIO_IN_CONFIG(ULTRA_PORT, HT_ULTRA_PORT, ULTRA_ECHO);
	
}

//******************************************************************
//��������  Temperature_Init
//���ܣ�     �������ģ���ʼ��
//��������� ��
//����ֵ��  ��
//���ڣ�  2016.04.06
//������	����
//******************************************************************
void Temperature_Init(void)
{
	Ultran_Init();
	current_temp.baby_temperature = 2500;
	current_temp.room_temperature = 2500;
}
//******************************************************************
//��������  int_to_str
//���ܣ�     �¶�*100 ת����ȷ���ַ���
//��������� s:�¶�*100��ֵ  buf: ���ڴ洢�Ļ���  size:��������С
//����ֵ��  Ŀ���¶�*100��ֵ
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static u8 int_to_str(int s, u8 buf[], u32 size)
{
	int i, val;
	if(s > 0)
	{
		for(i = size-1; i>=0; i--)
		{
			if(i==2)
			{
				buf[i]='.';
				continue;
			}
			val = s%10;
			s /= 10;
			buf[i] = val+0x30;
			if(s == 0) break;
		}
		return i;
	}
	return 0;
}

//******************************************************************
//��������  get_to
//���ܣ�    ��ȡĿ���¶�
//���������ģ�鷵�ص��ַ���
//����ֵ��  Ŀ���¶�*100��ֵ
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static int get_to(u8 buf[])
{
	if(buf[0] == 0x5A && buf[1] == 0x5A && buf[2] == 0x45 && buf[3] == 0x04)
	{
		return buf[4]<<8 | buf[5];
	}
	return -1;
}
//******************************************************************
//��������  get_ta
//���ܣ�    ��ȡ����
//���������ģ�鷵�ص��ַ���
//����ֵ��  ����*100��ֵ
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static int get_ta(u8 buf[])
{
	if(buf[0] == 0x5A && buf[1] == 0x5A && buf[2] == 0x45 && buf[3] == 0x04)
	{
		return buf[6]<<8 | buf[7];
	}
	return -1;
}

//******************************************************************
//��������  get_temperature
//���ܣ�    ���¶�ģ�Ȳ�ѯ�¶�
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static void get_temperature(u8 *buf)
{
	u8 check[3]={0xA5,0x15,0xBA};
	SEND_CHERK_CMD(check);
	delay_ms(10);
	GET_CHERK_DATA(buf);
}

//******************************************************************
//��������  send_temperature
//���ܣ�    �ѻ�õ��¶�ֵ���ͳ�ȥ
//�����������
//����ֵ��  ��
//���ڣ�  2016.03.21
//������	����
//******************************************************************
static struct TEMPERATURE read_temperature(void)
{
	int r;
	float tem, dis;
	int s;
	struct TEMPERATURE tempe;
	u8 buf[T_DATA_LEN] ={0};
	u8 sbuf[5]={0};
	u8 a[3] ="\r\n";
	u8 b[8]="target:";
	u8 c[8]="normal:";
	get_temperature(buf);
	
	OSTimeDly(10);
	
	//Ŀ���¶�
	r = get_to (buf);
//	printf("tem.%.2x %.2x\r\n", buf[6], buf[7]);
	if(r != -1)
	{
		s=r;
		tem = ((float)r)/100; 
//		Ultra_Ranging(&dis);
//		dis = infer(dis);
//		printf("%f ", dis);
		tem = check(tem, dis);
		tempe.baby_temperature = (int)(tem*100);
//		s = (int)(tem*100);
//		printf("temp :%f ", tem);
//		r = int_to_str(s, sbuf, 5);
//		Wifi_Send(b, 7);
//		Wifi_Send(sbuf, 5);
//		Wifi_Send(a, 2);
	}
	
	//����
	r = get_ta(buf);
	if(r != -1)
	{
		s=r;
		tempe.room_temperature = s;
//		r = int_to_str(s, sbuf, 5);
//		Wifi_Send(c, 7);
//		Wifi_Send(sbuf, 5);
//		Wifi_Send(a, 2);
	}
	return tempe;
}

u8 is_legal(struct TEMPERATURE *t)
{
	if(current_temp.room_temperature == 2500) return 1;
	if(t->baby_temperature > 6000) 
				return 0;
	if(t->room_temperature < 0) 
				return 0;
	if(t->room_temperature > 5000) 
				return 0;
	if((t->room_temperature - current_temp.room_temperature) > 15)
				return 0;
	if((t->room_temperature - current_temp.room_temperature) < -15)
				return 0;
	return 1;
}

//******************************************************************
//��������  check_temperature
//���ܣ�    ��������¶�
//�����������
//����ֵ��  �¶ȸı䣺��ǰ�¶ȣ� ������0��0
//���ڣ�  2016.03.21
//������	����
//******************************************************************
struct TEMPERATURE Check_Temperature(void)
{
	struct TEMPERATURE get_temp;
	
	get_temp = read_temperature();
	if(!is_legal(&get_temp))
	{
		return current_temp;
	}
	current_temp.baby_temperature = get_temp.baby_temperature;
	current_temp.room_temperature = get_temp.room_temperature;
	return get_temp;
}

u16 diff_temp(struct TEMPERATURE *t)
{
	return t->baby_temperature > t->room_temperature? 
		(t->baby_temperature - t->room_temperature):  (t->room_temperature-t->baby_temperature);
}

u8 is_onbed(struct TEMPERATURE *t)
{
	if(diff_temp(t)<200) return 0;
	if(t->baby_temperature < t->room_temperature) return 0;
	return 1;
}

/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 float *p:��໺�����
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i;
	u32 j;
	float Ultr_Temp;	
	for(i=0;i<5;i++)
	{
		TRIG_Send(1);
		delay_ms(10);
		TRIG_Send(0);
		while(!(ECHO_Reci));
		while(ECHO_Reci)
		{
			delay_us(2);
			j++;
		}
		Ultr_Temp+=340/2*j*2;//  ģ�����ɲ��3m 
		j=0;
		delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	*p=Ultr_Temp/5/1000000; 	
}

// ����ȡֵת��Ϊ����
float infer(float data) {
	return data/0.09*10;
}
static double pow(double d, int t)
{
	int i=0;
	double result=1;
	for(i = 0; i<t; i++)
	{
		result *= d;
	}
	return result;
}

// �����������
double check(double tem, double distance) {
	double ret = 0;
	ret = 0.00029*pow(distance, 2)+0.00003*tem*distance+0.985*tem;
	return ret;
}
