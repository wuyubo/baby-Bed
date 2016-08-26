#include "sem.h"
#include "led.h"

//extern volatile INT8U WlanStatus; 	//WIFI���ӳɹ���־
//extern volatile INT8U SocketStatus; //SOCKET���ӳɹ���־

volatile u8 Read_state;

static OS_EVENT *task_sem;
static OS_EVENT *read_sem;
static OS_EVENT *Wlan_sem;
static OS_EVENT *Socket_sem;

static OS_EVENT *RecvBuff_sem;
static OS_EVENT *RecvBuff_Read_sem;

//******************************************************************
//��������  task_sem_init
//���ܣ�    �ź����ĳ�ʼ��
//�����������
//����ֵ��  ��
//���ڣ�  2016.04.06
//������	����
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
//��������  lock_sem
//���ܣ�    ��ȡ�ź���
//�����������
//����ֵ��  ��
//���ڣ�  2016.04.06
//������	����
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
//��������  unlock_sem
//���ܣ�    �ͷ��ź���
//�����������
//����ֵ��  ��
//���ڣ�  2016.04.06
//������	����
//******************************************************************
void unlock_sem(OS_EVENT *_sem)
{
		OSSemPost(_sem);
}

///////////////////////////////////////////////////////////////////
//******************************************************************
//��������  run_led
//���ܣ�    LED��˸����
//�����������
//����ֵ��  ��
//���ڣ�  2016.04.06
//������	����
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
//��������  getReadStatu
//���ܣ�    SPIWIFI���ձ��
//�������
//����ֵ��  
//����
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
//��������  get_RecvBuff  put_RecvBuff
//���ܣ�    ���սӻ�����ź���
//�������
//����ֵ��  
//����
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
//��������  get_WlanSem   put_WlanSem
//���ܣ�  	wifi�˿ڻ���
//�������
//����ֵ��  
//���� 2016.05.03
//******************************************************************
INT8U get_WlanSem(void)
{
	return lock_sem(Wlan_sem);
}
void put_WlanSem(void)
{
		unlock_sem(Wlan_sem);
}






