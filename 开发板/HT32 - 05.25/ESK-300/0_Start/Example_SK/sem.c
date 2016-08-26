#include "sem.h"
#include "led.h"

//extern volatile INT8U WlanStatus; 	//WIFI连接成功标志
//extern volatile INT8U SocketStatus; //SOCKET连接成功标志

volatile u8 Read_state;

static OS_EVENT *task_sem;
static OS_EVENT *read_sem;
static OS_EVENT *Wlan_sem;
static OS_EVENT *Socket_sem;

static OS_EVENT *RecvBuff_sem;
static OS_EVENT *RecvBuff_Read_sem;

//******************************************************************
//函数名：  task_sem_init
//功能：    信号量的初始化
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
void task_sem_init(void)
{
	task_sem = OSSemCreate(1);
	read_sem = OSSemCreate(1);
	RecvBuff_sem = OSSemCreate(1);
	RecvBuff_Read_sem = OSSemCreate(1);
	Wlan_sem = OSSemCreate(1);
}

//******************************************************************
//函数名：  lock_sem
//功能：    获取信号量
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
INT8U lock_sem(OS_EVENT *_sem)
{
	INT8U err;
	u32 time = 0;
	while(1)
	{
		OSSemPend(_sem, 10, &err);
		if(err == OS_NO_ERR) return OS_NO_ERR;
		OSTimeDly(100);
		time ++;
		if(time >= SEM_TIMEOUT) return SEM_ERR;
	}
}

//******************************************************************
//函数名：  unlock_sem
//功能：    释放信号量
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
void unlock_sem(OS_EVENT *_sem)
{
		OSSemPost(_sem);
}

///////////////////////////////////////////////////////////////////
//******************************************************************
//函数名：  run_led
//功能：    LED闪烁测试
//输入参数：无
//返回值：  无
//日期：  2016.04.06
//姓名：	吴玉波
//******************************************************************
void run_led(uint32_t led, u32 time)
{
		lock_sem(task_sem);
		led_on(led);
		OSTimeDly(time);
		led_off(led);
		unlock_sem(task_sem);
		OSTimeDly(time);
}

//******************************************************************
//函数名：  getReadStatu
//功能：    SPIWIFI接收标记
//输入参数
//返回值：  
//吴玉波
//******************************************************************
u8 getReadStatu(void)
{
	u8 state;
	lock_sem(read_sem);
	state = Read_state;
	unlock_sem(read_sem);
	return state;
}
void setReadStatu(u8 state)
{
	lock_sem(read_sem);
	Read_state = state;
	unlock_sem(read_sem);
}

//******************************************************************
//函数名：  get_RecvBuff  put_RecvBuff
//功能：    接收接缓冲的信号量
//输入参数
//返回值：  
//吴玉波
//******************************************************************
INT8U get_RecvBuff(u8 lock)
{
	if(lock == 1)
		return lock_sem(RecvBuff_sem);
	else if(lock == 2)
		return lock_sem(RecvBuff_Read_sem);
	return SEM_ERR;
}
void put_RecvBuff(u8 lock)
{
	if(lock == 1)
		unlock_sem(RecvBuff_sem);
	else if(lock == 2)
		unlock_sem(RecvBuff_Read_sem);
}

//******************************************************************
//函数名：  get_WlanSem   put_WlanSem
//功能：  	wifi端口互斥
//输入参数
//返回值：  
//吴玉波 2016.05.03
//******************************************************************
INT8U get_WlanSem(void)
{
	return lock_sem(Wlan_sem);
}
void put_WlanSem(void)
{
		unlock_sem(Wlan_sem);
}






