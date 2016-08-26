/*
 * bed_thread.c
 *
 *  Created on: 2016-4-12
 *      Author: train
 */

#include "bed_thread.h"
#include "bed_types.h"
#include "bed_server.h"
#include "phone_thread.h"
#include "cmysql/cmysql.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

//#define BED_DEBUG


pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;


int _get_bed_status(int client_fd , struct DATA_PACKET *dpacket);

int _store_bed_status(struct DATA_PACKET *dpacket);

int send_respone(int client_fd);

int _send_command_to_bed(int client_fd, struct COMMAND_PACKET *cpaket);

int _get_command_from_bed(int client_fd);


void *client_thread_bed(void *arg)
{
	struct COMMAND_PACKET cpaket;
	struct DATA_PACKET dpacket;
	int client_fd = ((struct TCP_THREAD_ARG *)arg)->sockfd;
	struct	sockaddr_in addr = ((struct TCP_THREAD_ARG *)arg)->sockaddr;
	char *ip = (char *)inet_ntoa(addr.sin_addr);
	int task;

#ifndef BED_DEBUG
	printf("4.[%s]client_thread_bed\n", ip);
#endif
	loger(BED, STDOUT, "client_thread_bed");
	task = _get_command_from_bed(client_fd);

	if(task == TASK_CMD || task == TASK_DATA)
	{
#ifdef BED_DEBUG		
		printf("5.send command\n");
#endif	
		get_command_from_mysql(&cpaket);
		_send_command_to_bed(client_fd, &cpaket);

#ifdef BED_DEBUG	
		printf("get data\n");
#endif	
		_get_bed_status(client_fd, &dpacket);
		send_respone(client_fd);
		_store_bed_status(&dpacket);
	}

	close(client_fd);
#ifdef BED_DEBUG
	printf("6.client disconnect.\n");
#endif
	return 0;
}

/*store data*/
int _store_bed_status(struct DATA_PACKET *dpacket)
{
	char *buf = &dpacket->data;
#ifdef BED_DEBUG
	printf("Header 0x%.2x 0x%.2x 0x%.4x 0x%.4x\n", dpacket->header.ck1, dpacket->header.ck2 | dpacket->header.type, dpacket->header.length, sizeof(struct BED_DATA));
//	printf("data   0x%.2x%.2x%.2x%.2x 0x%.2x%.2x%.2x%.2x\n", buf[0],buf[1], buf[2],buf[3], buf[4], buf[5],buf[6], buf[7]);
	printf(" rtemp=%d, btemp=%d, ", dpacket->data.room_temperature, dpacket->data.baby_temperature);
	//if(dpacket->data.status.pee) printf("pissing\n");
	//else printf("not piss\n");
#endif
	save_data_to_mysql(dpacket);
	loger(BED, STDOUT, "store_bed_status");
	return 1;
}

/*Receive bed data from client.*/
int _get_bed_status(int client_fd , struct DATA_PACKET *dpacket)
{
	
	bzero((char *)dpacket, sizeof(dpacket));
	int recvbytes = recv(client_fd, dpacket, sizeof(struct DATA_PACKET), 0);
	printf(" recvLength = %d \n", recvbytes);
	if(recvbytes < 0) {
		perror("socket: recv\n");
		loger(BED, STDERR, "err:get_bed_status");
		return -1;
	}
	return 0;
}

int update_status(struct DATA_PACKET *cpacket)
{
	char query[255];
	sprintf(query,"update Status set kick=%d, heat=%d,  onbed=%d, cry=%d, \
		swing=%d, pee=%d, play=%d, fan=%d, room_Temp=%d , baby_Temp=%d where \
		Bed_Guid=(select Guid from BabyBed where Mac = '5E:60:7F:9A:CC:91')",
		cpacket->data.status.kick, cpacket->data.status.heat,
		cpacket->data.status.onbed, cpacket->data.status.cry,
		cpacket->data.status.swing, cpacket->data.status.pee,
		cpacket->data.status.play,cpacket->data.status.fan, 
		cpacket->data.room_temperature,
		cpacket->data.baby_temperature);
		
	
	Cmysql_init(SERVER, USER, PASSWARD, DATABASE);
	if (!connectMysql())
    {
        fprintf(stderr, "%s\n", sql_error());
       return -1;
    }
	if (Cmysql_query(query)) 
    {
        fprintf(stderr, "%s\n", sql_error());
		loger(BED, STDERR, "err:update_status");
		loger(BED, STDERR, sql_error());
		
        return -1;
    }
	Cmysql_release();
	
#ifdef BED_DEBUG	
	printf("%s\n",query);
#endif
	loger(BED, STDOUT, query);
	return 0;
}

int save_data_to_mysql(struct DATA_PACKET *cpacket)
{
	char query[255];
	/* database mysql*/
	
	update_status(cpacket);
	loger(BED, STDOUT, "save_data_to_mysql");
	return 0;
}



int get_command_from_mysql(struct COMMAND_PACKET *cmd)
{
	SQLResult result;
	int j;
	struct COMMAND_PACKET tmp;
	tmp.cmd.ctr = 0;
	tmp.cmd.cmd = 0;
	/* database mysql*/
	Cmysql_init(SERVER, USER, PASSWARD, DATABASE);
	if (!connectMysql())
    {
        fprintf(stderr, "%s\n", sql_error());
       return -1;
    }
	
	if (Cmysql_query("select control, cmd from Command , \
		BabyBed Where BabyBed.Mac = '5E:60:7F:9A:CC:91' and \
		BabyBed.Guid = Command.Guid")) 
    {
        fprintf(stderr, "%s\n", sql_error());
		loger(BED, STDERR, "err:get_command_from_mysql");
		loger(BED, STDERR, sql_error());
        return -1;
    }
    Cmysql_result(&result);
	while ((result.row = Cmysql_result_next(&result)) != NULL)
	 {
		 if(result.field_count>0)
		 {
			cmd->cmd.ctr = Cmysql_row_to_int((char *)result.row[0]);
			cmd->cmd.cmd = Cmysql_row_to_int((char *)result.row[1]);
		 }	
#ifdef BED_DEBUG	
		  for(j=0; j<result.field_count; j++)
		 {
			 printf("%s \n", result.row[j]);
		 }	
#endif
	 }
	Cmysql_free_result(&result);
	Cmysql_release();
	loger(BED, STDOUT, "get_command_from_mysql");
	return save_command_to_mysql(&tmp);

}

/*send_respone*/
int send_respone(int client_fd)
{
	int sendbytes;
	struct COMMAND_PACKET cpacket;
	cpacket.header.ck1 = 0xFB;
	cpacket.header.ck2 = 0x0A;
	cpacket.header.type = TYPE_BED;
	cpacket.header.length=sizeof(struct CMD);
	cpacket.cmd.ctr = PLAY;
	cpacket.cmd.cmd = 1;
	
	sendbytes = send(client_fd, &cpacket, sizeof(struct COMMAND_PACKET), 0);

	//printf("cmd=%x ", (int)cpacket.cmd);

	cpacket.cmd.cmd = NONE;
	//command.cmd = NONE;

	if(sendbytes < 0) {
		perror("socket: send.\n");
		loger(BED, STDERR, "err:send_respone");
		return 0;
	}
	loger(BED, STDOUT, "send_respone");
	return 1;
}

/*send command to bed.*/
int _send_command_to_bed(int client_fd, struct COMMAND_PACKET *cpacket)
{
	int sendbytes;
	sendbytes = send(client_fd, cpacket, sizeof(struct COMMAND_PACKET), 0);
#ifdef BED_DEBUG	
	printf("send cmd=%d \n", cpacket->cmd.cmd);
#endif
	if(sendbytes < 0) {
		perror("socket: send.\n");
		loger(BED, STDERR, "err:send_command_to_bed");
		return 0;
	}
	loger(BED, STDOUT, "send_command_to_bed");
	return 1;
}

/*get command to bed.*/
int _get_command_from_bed(int client_fd)
{
	int readbytes;
	struct COMMAND_PACKET cpacket;
	readbytes = recv(client_fd, &cpacket, sizeof(struct COMMAND_PACKET), 0);
#ifdef BED_DEBUG	
	printf("cmd=%x\n ", cpacket.cmd.cmd);
#endif
	if(readbytes < 0) {
		perror("socket: send.\n");
		loger(BED, STDERR, "err:get_command_from_bed");
		return -1;
	}
	loger(BED, STDOUT, "get_command_from_bed");
	return cpacket.cmd.cmd;
}




