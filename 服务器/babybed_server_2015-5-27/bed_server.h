/*
 * bed_server.h
 *
 *  Created on: 2016-4-5
 *      Author: Lin Yixing
 */

#ifndef BED_SERVER_H_
#define BED_SERVER_H_

#include "bed_types.h"
#include <netinet/in.h>
/*the maximum number of the concurrent requests*/
#define MAX_QUE_CONN_NM 5
#define SOCK_BUF_SIZE 1024

/*returned values of socket-operate*/
#define OPR_SUCCESS 0
#define CONN_ERR   -1
#define BIND_ERR   -2
#define LISTEN_ERR -3
#define ACCEPT_ERR -4
#define THREAD_ERR -5
#define DEVICE_ERR -6


#define PHONE 1
#define BED 2
#define STDOUT 2
#define STDERR 1


struct TCP_THREAD_ARG {
	int sockfd;		//嵌套字连接符
	struct sockaddr_in sockaddr;	//ip地址
	int16_t port;
};

int socket_initialize(int16_t port);
int16_t socket_accept(int sockfd, int16_t port);

void loger(int type, int streamType, int *msg);

#endif /* BED_SERVER_H_ */
