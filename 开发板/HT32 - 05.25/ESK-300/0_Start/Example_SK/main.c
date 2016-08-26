
/* Includes ------------------------------------------------------------------------------------------------*/
#include "main.h"

//分配堆栈
static OS_STK Task_DealData_stk[TASK_STK_SIZE];
static OS_STK Task_RecvData_stk[TASK_STK_SIZE];
static OS_STK Task_DealRock_stk[TASK_STK_SIZE_SMALL];
//static OS_STK Task_DealCommadData_stk[TASK_STK_SIZE_SMALL];

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
/**************************************************/
void CKCU_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Config(void);
void Modules_Init(void);
/**************************************************/
//定时器初始化
static void systick_init(void)
{
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK); // 9MHz on chip
  SYSTICK_SetReloadValue(9000000/OS_TICKS_PER_SEC);              
  SYSTICK_IntConfig(ENABLE);
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
}

/**************************************************/
//初始任务
static void startup_task(void* p_arg)
{
	INT8U err;
	systick_init();
#if (OS_TASK_STAT_EN > 0)
	OSStatInit();/* Determine CPU capacity. */
#endif
	//创建线程
	err = OSTaskCreate(Task_DealData, (void *)0, &Task_DealData_stk[TASK_STK_SIZE-1], TASK_DEALDATA_PRIO);
	err = OSTaskCreate(Task_RecvData, (void *)0, &Task_RecvData_stk[TASK_STK_SIZE-1], TASK_RECVDATA_PRIO);
	err = OSTaskCreate(Task_DealRock, (void *)0, &Task_DealRock_stk[TASK_STK_SIZE_SMALL-1], TASK_DEALPTCS_PRIO);
//	err = OSTaskCreate(Task_DealCommadData, (void *)0, &Task_DealCommadData_stk[TASK_STK_SIZE_SMALL-1], TASK_DEALPTCS_PRIO);
	if (OS_ERR_NONE != err)
		while(1)
			;
	OSTaskDel(OS_PRIO_SELF); 
}

int main(void)
{
	OSInit();														/*OS init*/
	CKCU_Configuration();               /* System Related configuration                                       */
	NVIC_Configuration();
	EXTI_Config();
	led_init();
	uart1_init();
	task_sem_init();
	Modules_Init();
	
	OSTaskCreate(startup_task, (void*) 0, &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);
	//启动OS
	printf("starting\r\n");
	OSStart();
	return 0;
}

//模块初始化
void Modules_Init(void)
{
	Wifi_Init();
	Temperature_Init();
	Voice_Init();
	Music_Init();
	Fan_Init();
	Piss_Init();
	Quilt_Init();
	Rock_Init();
	Onbed_Init();
	system_Init();
	client_Init();
}

//时钟初始化
void CKCU_Configuration(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.PDMA       = 0;
  CKCUClock.Bit.USBD       = 0;
  CKCUClock.Bit.CKREF      = 0;
  CKCUClock.Bit.EBI        = 0;
  CKCUClock.Bit.CRC        = 0;
  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;
  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.PD         = 1;
  CKCUClock.Bit.PE         = 1;
  CKCUClock.Bit.I2C0       = 0;
  CKCUClock.Bit.I2C1       = 0;
  CKCUClock.Bit.SPI0       = 0;
  CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 0;
  CKCUClock.Bit.USART1     = 0;
  CKCUClock.Bit.UART0      = 0;
  CKCUClock.Bit.UART1      = 0;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.EXTI       = 1; 
  CKCUClock.Bit.SCI        = 0;
  CKCUClock.Bit.I2S        = 0;
  CKCUClock.Bit.MCTM0      = 0;
  CKCUClock.Bit.MCTM1      = 0;
  CKCUClock.Bit.WDT        = 0;
  CKCUClock.Bit.BKP        = 0;
  CKCUClock.Bit.GPTM0      = 0;
  CKCUClock.Bit.GPTM1      = 0;
  CKCUClock.Bit.BFTM0      = 0;
  CKCUClock.Bit.BFTM1      = 0;
  CKCUClock.Bit.OPA0       = 0;
  CKCUClock.Bit.OPA1       = 0;
  CKCUClock.Bit.ADC        = 0;
	CKCUClock.Bit.BKP        = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x0);     /* Set the Vector Table base location at 0x00000000   */
}

//外部中断配置
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	GPIO_PullResistorConfig(HT_GPIOE, GPIO_PIN_1, GPIO_PR_UP);	
	AFIO_GPEConfig(AFIO_PIN_1, AFIO_MODE_1);	
	GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_1, GPIO_DIR_IN);	
	GPIO_InputConfig(HT_GPIOE, GPIO_PIN_1, ENABLE);
	
	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_1;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;//下降沿中断
	EXTI_InitStruct.EXTI_DebounceCnt = 720000; 
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/* Configure alternated mode 1 of GPIO                                                                    */
  AFIO_GPCConfig(GPIO_PIN_1, AFIO_MODE_1);
	/* Select Port as EXTI Trigger Source                                                                     */
  AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PE);
	EXTI_IntConfig(EXTI_CHANNEL_1, ENABLE); 
  /* Enable EXTI_CHANNEL_10 NVIC Interrupt Channel                                                          */
  NVIC_EnableIRQ(EXTI1_IRQn);
	
	
	GPIO_PullResistorConfig(HT_GPIOE, GPIO_PIN_0, GPIO_PR_UP);	
	AFIO_GPEConfig(AFIO_PIN_0, AFIO_MODE_1);	
	GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_0, GPIO_DIR_IN);	
	GPIO_InputConfig(HT_GPIOE, GPIO_PIN_0, ENABLE);
	
	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;//下降沿中断
	EXTI_InitStruct.EXTI_DebounceCnt = 720000; 
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/* Configure alternated mode 1 of GPIO                                                                    */
  AFIO_GPCConfig(GPIO_PIN_0, AFIO_MODE_1);
	/* Select Port as EXTI Trigger Source                                                                     */
  AFIO_EXTISourceConfig(AFIO_EXTI_CH_0, AFIO_ESS_PE);
	EXTI_IntConfig(EXTI_CHANNEL_0, ENABLE); 
  /* Enable EXTI_CHANNEL_10 NVIC Interrupt Channel                                                          */
  NVIC_EnableIRQ(EXTI0_IRQn);
}

void system_Reset(void)
{
	u8 ret = 0;
	do
	{
		Wifi_ForceReset();
		delay_ms(3000);
		ret =  Wifi_Connect();
	}while(ret);
	printf("wifi reset\r\n");
	//NVIC_SystemReset();
	//main();
	//Reset_Handler();
}

