#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include "main.h"

#define BUFFSIZE 100
#define EMPTY    0
#define NOTEMPTY 1
#define FULL     2
#define SYS_ERR 102
/**
 * NOTE: BUFFER
 */
struct BUFFER
{
	u8 state;
	u32 offset;
	u32 end;
	u8 *buff;
};

void system_Init(void);
void clear_grobalBuff(void);
int send(int fd, u8 *buff, u32 size, u8 state);
int read(int fd, u8 *buff, u32 size, u8 state);
int Save_ToBuffer(u8 *buff, u32 size);

#endif


