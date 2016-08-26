#include "gy-mcu90615.h"
struct TEMPERATURE current_temp;
//******************************************************************
//函数名：  Ultran_Init
//功能：     超声波模块初始化
//输入参数： 无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
static void Ultran_Init(void)
{
	//trig输出
	GPIO_OUT_CONFIG(ULTRA_PORT, HT_ULTRA_PORT, ULTRA_TRIG);
	//echo 输入
	GPIO_IN_CONFIG(ULTRA_PORT, HT_ULTRA_PORT, ULTRA_ECHO);
	
}

//******************************************************************
//函数名：  Temperature_Init
//功能：     人体红外模块初始化
//输入参数： 无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
void Temperature_Init(void)
{
	Ultran_Init();
	current_temp.baby_temperature = 2500;
	current_temp.room_temperature = 2500;
}
//******************************************************************
//函数名：  int_to_str
//功能：     温度*100 转成正确的字符串
//输入参数： s:温度*100数值  buf: 用于存储的缓冲  size:缓冲区大小
//返回值：  目标温度*100数值
//日期：  2016.03.21
//姓名：	吴玉波
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
//函数名：  get_to
//功能：    获取目标温度
//输入参数：模块返回的字符串
//返回值：  目标温度*100数值
//日期：  2016.03.21
//姓名：	吴玉波
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
//函数名：  get_ta
//功能：    获取室温
//输入参数：模块返回的字符串
//返回值：  室温*100数值
//日期：  2016.03.21
//姓名：	吴玉波
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
//函数名：  get_temperature
//功能：    向温度模度查询温度
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
//******************************************************************
static void get_temperature(u8 *buf)
{
	u8 check[3]={0xA5,0x15,0xBA};
	SEND_CHERK_CMD(check);
	delay_ms(10);
	GET_CHERK_DATA(buf);
}

//******************************************************************
//函数名：  send_temperature
//功能：    把获得的温度值发送出去
//输入参数：无
//返回值：  无
//日期：  2016.03.21
//姓名：	吴玉波
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
	
	//目标温度
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
	
	//室温
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
//函数名：  check_temperature
//功能：    检测人体温度
//输入参数：无
//返回值：  温度改变：当前温度， 否则：清0，0
//日期：  2016.03.21
//姓名：	吴玉波
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
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 float *p:测距缓存变量
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
		Ultr_Temp+=340/2*j*2;//  模块最大可测距3m 
		j=0;
		delay_ms(60);//防止发射信号对回响信号的影响
	}
	*p=Ultr_Temp/5/1000000; 	
}

// 将读取值转换为厘米
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

// 误差修正函数
double check(double tem, double distance) {
	double ret = 0;
	ret = 0.00029*pow(distance, 2)+0.00003*tem*distance+0.985*tem;
	return ret;
}
