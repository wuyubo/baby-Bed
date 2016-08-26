/*
 * phone_thread.c
 *
 *  Created on: 2016-4-7
 *      Author: train
 */
#include "phone_thread.h"
#include "bed_thread.h"
#include "bed_server.h"
#include "cmysql/cmysql.h"


#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>


#define PHONE_DEBUG


int _send_data_to_phone(int client_fd, struct DATA_PACKET *dpacket);
struct COMMAND_PACKET _get_phone_command(int client_fd);

/* Thread for phone.
 * */
void *client_thread_phone(void *arg)
{
	int client_fd = ((struct TCP_THREAD_ARG *)arg)->sockfd;
	struct	sockaddr_in addr = ((struct TCP_THREAD_ARG *)arg)->sockaddr;
	char *ip = (char *)inet_ntoa(addr.sin_addr);

	struct DATA_PACKET dpacket;
#ifndef PHONE_DEBUG
	printf("Client connected. sockid = %d, ip = %s, tid = %lu\n", \
			client_fd, ip, pthread_self());
#endif
	loger(PHONE, STDOUT, "client_thread_bed");
	/*get command from phone.*/
	struct COMMAND_PACKET cpacket = _get_phone_command(client_fd);
	if(cpacket.cmd.ctr != 5 && cpacket.cmd.ctr != 0)
		save_command_to_mysql(&cpacket);
	if(cpacket.cmd.ctr == 5)
	{
		//printf("send data\n");
		get_data_from_mysql(&dpacket);
		/*send bed-data to phone.*/
		_send_data_to_phone(client_fd, &dpacket);
	}

	//printf("client disconnected.\n");
	close(client_fd);
	return 0;
}


int get_data_from_mysql(struct DATA_PACKET *cpacket)
{
	SQLResult result;
	int j;
	/* database mysql*/
	Cmysql_init(SERVER, USER, PASSWARD, DATABASE);
	if (!connectMysql())
    {
        fprintf(stderr, "%s\n", sql_error());
       return -1;
    }
	if (Cmysql_query("select * from Status , \
		BabyBed Where BabyBed.Mac = '5E:60:7F:9A:CC:91' and \
		BabyBed.Guid = Status.Guid")) 
    {
        fprintf(stderr, "%s\n", sql_error());
		loger(BED, STDERR, "err:get_data_from_mysql");
		loger(BED, STDERR, sql_error());
        return -1;
    }
    Cmysql_result(&result);
	while ((result.row = Cmysql_result_next(&result)) != NULL)
	 {
		 if(result.field_count>0)
		 {
			 cpacket->data.status.kick = Cmysql_row_to_int((char *)result.row[1]);
			 cpacket->data.status.heat = Cmysql_row_to_int((char *)result.row[2]);
			 cpacket->data.status.onbed = Cmysql_row_to_int((char *)result.row[3]);
			 cpacket->data.status.cry =  Cmysql_row_to_int((char *)result.row[4]);
			 cpacket->data.status.swing =  Cmysql_row_to_int((char *)result.row[5]);
			 cpacket->data.status.pee =  Cmysql_row_to_int((char *)result.row[6]);
			 cpacket->data.status.play =  Cmysql_row_to_int((char *)result.row[7]);
			 cpacket->data.status.fan =  Cmysql_row_to_int((char *)result.row[8]);
			 cpacket->data.room_temperature = Cmysql_row_to_int((char *)result.row[9]);
			 cpacket->data.baby_temperature =  Cmysql_row_to_int((char *)result.row[10]);
		 }	
#ifdef PHONE_DEBUG
		   for(j=0; j<result.field_count; j++)
		  {
			  printf("%s ", result.row[j]);
		  }
		  printf("-------------------\n");
#endif
	 }
	 Cmysql_free_result(&result);
	 Cmysql_release();
	 loger(PHONE, STDOUT, "get_data_from_mysql");
}

int save_command_to_mysql(struct COMMAND_PACKET *cpacket)
{
	char query[255];
	/* database mysql*/
	Cmysql_init(SERVER, USER, PASSWARD, DATABASE);
	if (!connectMysql())
    {
        fprintf(stderr, "%s\n", sql_error());
       return -1;
    }
	sprintf(query,"update Command set control=%d, cmd=%d where \
		Bed_Guid=(select Guid from BabyBed where Mac = '5E:60:7F:9A:CC:91')",
		cpacket->cmd.ctr, cpacket->cmd.cmd);
	if (Cmysql_query(query)) 
    {
        fprintf(stderr, "%s\n", sql_error());
		loger(BED, STDERR, "err:save_command_to_mysql");
		loger(BED, STDERR, sql_error());
        return -1;
    }
	 Cmysql_release();
#ifndef PHONE_DEBUG
	printf("%s\n",query);
#endif
	loger(BED, STDOUT, query);
	return 0;
}
/*
 * NOTE: Send command packet to baby-bed, and return real-time data from baby-bed.
 * */

struct COMMAND_PACKET _get_phone_command(int client_fd)
{
	struct COMMAND_PACKET cpacket;
	bzero(&cpacket, sizeof(struct COMMAND_PACKET));
	int recvbytes;
	recvbytes = recv(client_fd, &cpacket, sizeof(struct COMMAND_PACKET), 0);
	if(recvbytes < 0) {
		perror("socket: recv.\n");
		loger(PHONE, STDERR, "err:get_phone_command");
		return cpacket;
	}
	loger(PHONE, STDOUT, "get_phone_command");
	return cpacket;
}

/*Send data packet to client*/
int _send_data_to_phone(int client_fd, struct DATA_PACKET *dpacket)
{
	int sendbytes;
	sendbytes = send(client_fd, dpacket, sizeof(struct DATA_PACKET), 0);
	if(sendbytes < 0) {
		perror("socket: send.\n");
		loger(PHONE, STDERR, "err:send_data_to_phone failed");
		return -1;
	}
	if(dpacket->data.baby_temperature != 0)
	{
		dpacket->data.baby_temperature = 0;
		update_status(dpacket);
	}
	loger(PHONE, STDOUT, "send_data_to_phone");
	return 1;
}





