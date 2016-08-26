#include "system.h"

static u8 packet_buffer[BUFFSIZE];
static struct BUFFER Grobal_buff;

int cpytobuf(const u8* source, u32 size);
int cpyfrombuf(u8* source, u32 size);
void refresh_BuffState(void);
u8 Check_Buff(void);
void getbuff(void);
void releasebuff(void);
u8 Set_Buff(u8 state);
u8 Wait_GrobalBuff(u8 state, u32 timeout);

/*receive buffer init*/
void system_Init(void)
{
	Grobal_buff.state = EMPTY;
	Grobal_buff.offset = 0;
	Grobal_buff.end = 0;
	Grobal_buff.buff = packet_buffer;
}

/*clear grobal buffer*/
void clear_grobalBuff(void)
{
	system_Init();
	memset(Grobal_buff.buff, 0, BUFFSIZE);
}

/*copy to buf*/
int cpytobuf(const u8* source, u32 size)
{
	u32 i = 0;
	u32 end = Grobal_buff.end;
	u32 bufsize = BUFFSIZE-1;
	u32 j = 0;
	for(i=0; i<size; i++)
	{
		j = (i+end)% bufsize;
		Grobal_buff.buff[j] = source[i];
		if((j+1) == Grobal_buff.offset)
		{
			i++;
			break;
		}
	}
	j = (i+end)% bufsize;
	Grobal_buff.end = j;
	refresh_BuffState();
	return i;
}

/*copy from buf*/
int cpyfrombuf(u8* source, u32 size)
{
	u32 i = 0;
	u32 j = 0;
	u32 offset = Grobal_buff.offset;
	u32 bufsize = BUFFSIZE-1;
	for(; i<size; i++)
	{
		j = (i+offset)% bufsize;
		source[i] = Grobal_buff.buff[j];
		if((j+1) == Grobal_buff.end)
		{
			i++;
			break;
		}
	}
	j = (i+offset)% bufsize;
	Grobal_buff.offset = j;
	refresh_BuffState();
	return i;
}


/*check grobal buff*/
u8 Check_Buff(void)
{
	u8 state = EMPTY;
	if(get_RecvBuff(2)== SEM_ERR)
				return EMPTY;
	state = Grobal_buff.state;
	put_RecvBuff(2);
	return state;
}
/*using grobal buffer*/
void getbuff(void)
{
	get_RecvBuff(1);
}
/*release grobal buffer*/
void releasebuff(void)
{
	put_RecvBuff(1);
}

/*set grobal buff state*/
u8 Set_Buff(u8 state)
{
	if(get_RecvBuff(2)== SEM_ERR)
				return EMPTY;
	Grobal_buff.state = state;
	put_RecvBuff(2);
	return state;
}
/*refresh Buff State*/
void refresh_BuffState(void)
{
	if(Grobal_buff.end == Grobal_buff.offset)
		Set_Buff(EMPTY);
	else if(Grobal_buff.end+1 == Grobal_buff.offset)
		Set_Buff(FULL);
	else
		Set_Buff(NOTEMPTY);
}


/*wait grobal buff*/
u8 Wait_GrobalBuff(u8 state, u32 timeout)
{
	u16 retry = 0;
	while(Check_Buff() == state)
	{
		delay_ms(100);
		if(++retry == (timeout*10)) return SYS_ERR ;
	}
	return 0;
}

//******************************************************************
//函数名：  Save_ToBuffer
//功能：    把接收到的数据存到缓冲
//输入参数：
//返回值：  无
//日期：  2016.04.17
//姓名：	吴玉波
//******************************************************************
int Save_ToBuffer(u8 *buff, u32 size)
{
	if(Wait_GrobalBuff(FULL, 2) == SYS_ERR);
	getbuff();
	cpytobuf(buff, size);
	releasebuff();
	return size;
}


//******************************************************************
//函数名：  Read_FromBuff
//功能：    从缓冲中读数据出来
//输入参数：无
//返回值：  
//日期：  2016.04.17
//姓名：	吴玉波
//******************************************************************
int Read_FromBuff(u8 *buff, u32 size)
{
	if(Wait_GrobalBuff(EMPTY, 5) == CLIENT_ERR) return SYS_ERR;
	getbuff();
	cpyfrombuf(buff, size);
	releasebuff();
	return size;
}

/*send*/
int send(int fd, u8 *buff, u32 size, u8 state)
{
	 return Wifi_Send(fd, buff, size);
}

/*read*/
int read(int fd, u8 *buff, u32 size, u8 state)
{
	 return Read_FromBuff(buff, size);
}

