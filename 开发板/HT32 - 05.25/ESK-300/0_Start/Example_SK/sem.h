#ifndef _SEM_H_
#define _SEM_H_
#include "ht32.h"
#include "ht32_board.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#define SEM_ERR 101
#define SEM_TIMEOUT 50

void task_sem_init(void);
INT8U lock_sem(OS_EVENT *_sem);
void unlock_sem(OS_EVENT *_sem);

void run_led(uint32_t led, u32 time);
void setReadStatu(u8 state);
u8 getReadStatu(void);
INT8U get_RecvBuff(u8 lock);
void put_RecvBuff(u8 lock);
INT8U get_DataPacket(void);
void put_DataPacket(void);
INT8U get_CmdPacket(void);
void put_CmdPacket(void);
INT8U get_WlanSem(void);
void put_WlanSem(void);

#endif
