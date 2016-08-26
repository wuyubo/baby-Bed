#ifndef APP_CFG_H
#define APP_CFG_H
/* task priority */ 
#define STARTUP_TASK_PRIO         1
#define TASK_DEALDATA_PRIO     		6
#define TASK_RECVDATA_PRIO     		7

#define TASK_DEALPTCS_PRIO     	  8
#define Task_DealCommadData_PRIO  9
#define UREAD_TASK_PRIO     	  	10



/* task stack size */
#define STARTUP_TASK_STK_SIZE     60
#define TASK_STK_SIZE     	      128
#define TASK_STK_SIZE_SMALL     	70

#endif
