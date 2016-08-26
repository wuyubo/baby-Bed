#include "bed_server.h"
#include "bed_types.h"
#include "phone_thread.h"
#include "bed_thread.h"
#include "cmysql/cmysql.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

//#define DEBUG

struct DEVICE _get_device_identification(int client_fd);
int _send_device_mac_type(int client_fd, struct DEVICE * device);
void *client_thread_check(void *arg);

pthread_mutex_t loger_mutex = PTHREAD_MUTEX_INITIALIZER;


/* socket initialization.
 * This function contain the socket-operate of 'connect', 'bind' and 'listen'.
 * RETURN:  socket_id for initializing successfully.
 * */

int socket_initialize(int16_t port)
{
	int sockfd;
	struct sockaddr_in server_sockaddr;
	int sin_size = sizeof(struct sockaddr);

	/*	build socket connection
	 * */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	// IPV4, TCP
	if(sockfd == -1) {
		perror("Error: socket connect.\n");
		return CONN_ERR;
	}

	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(port);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero((server_sockaddr.sin_zero), 8);

	int i = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	/* socket bind
	 */
	if(bind(sockfd, (struct sockaddr*)&server_sockaddr, sin_size) == -1) {
		perror("Error: socket bind.\n");
		return BIND_ERR;
	}
	
	/* socket listen
	 */
	if(listen(sockfd, MAX_QUE_CONN_NM) == -1) {
		perror("Error: socket listen\n");
		return LISTEN_ERR;
	}
	
	
	return sockfd;
}

int16_t socket_accept(int sockfd, int16_t port)
{
	int client_fd;
	struct sockaddr_in client_sockaddr;
	struct TCP_THREAD_ARG  thread_arg;
	int sin_size = sizeof(struct sockaddr);
	pthread_t tid;
	pthread_attr_t a; //线程属性
	pthread_attr_init(&a);  //初始化线程属性
	pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);      //设置线程属性
	
	
	
	/*socket accept*/
	client_fd = accept(sockfd, (struct sockaddr *)&client_sockaddr, (socklen_t *)&sin_size);
	if(client_fd == -1) {
		perror("Error: accept.\n");
		return ACCEPT_ERR;
	}

	thread_arg.sockfd = client_fd;
	thread_arg.sockaddr = client_sockaddr;
	thread_arg.port = port;
#ifdef DEBUG	
	perror("1.client connect\n");
#endif
	/*create a thread.*/
	if (pthread_create(&tid, &a, client_thread_check, (void *)&thread_arg) != 0) {
				perror("Error: thread.\n");
				return THREAD_ERR;
	}
	//client_thread_check((void *)&thread_arg);
	return OPR_SUCCESS;
}

/*
 * NOTE: Get device's MAC and type. Type == 0xFF when recv() falled.
 * FUNCTION: Get 'DEVICE' from client and then sent 'DEVIVE' to client for response.
 *
 * */
struct DEVICE _get_device_identification(int client_fd)
{
	struct DEVICE device;
	int recvbyte, sendbytes;
	bzero(&device, sizeof(struct DEVICE));
	recvbyte = recv(client_fd, &device, sizeof(struct DEVICE), 0);
	if(recvbyte < 0) {
		perror("socket: recv\n");
		device.type = 0xff;
	}
	return device;
}

/*
 * NOTE: Send device's MAC and type. Type == 0xFF when recv() falled.
 * FUNCTION: Get 'DEVICE' from client and then sent 'DEVIVE' to client for response.
 *
 * */
int _send_device_mac_type(int client_fd, struct DEVICE * pdevice)
{
	int sendbytes;
	/*response to client.*/
	sendbytes = send(client_fd, pdevice, sizeof(struct DEVICE), 0);
	
	if(sendbytes < 0) {
		perror("socket: send.\n");
		pdevice->type = 0xff;
		return 0;
	}
	return 1;
}

void print_device(struct DEVICE * d)
{
	printf("--->devcice: %x:%x:%x:%x:%x:%x, type:%d \r\n", d->mac[0], d->mac[1], d->mac[2], d->mac[3], d->mac[4],
								d->mac[5], d->type);
}

void *client_thread_check(void *arg)
{
	int client_fd = ((struct TCP_THREAD_ARG *)arg)->sockfd;
	struct TCP_THREAD_ARG  *thread_arg = (struct TCP_THREAD_ARG *)arg;
	pthread_t tid;
	pthread_attr_t a; //线程属性
	pthread_attr_init(&a);  //初始化线程属性
	pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);      //设置线程属性
	/*Analyse get MAC and type from client*/
	struct DEVICE device;
	device = _get_device_identification(client_fd);
	if((device.type != TYPE_PHONE) && (device.type != TYPE_BED)) {
		perror("DEVICE: type.\n");
		goto CHECK_ERR;
	}
		/*send device back*/
#ifdef DEBUG
	printf("2.send devcice back len=%d\n", sizeof(device));
#endif
	_send_device_mac_type(client_fd, &device);

	
	/* create a thread*/
	switch(device.type) {
	case TYPE_PHONE:		/*Client's device is phone.*/
#ifdef DEBUG
		printf("3.This is a phone.\n");
		
#endif
		if(thread_arg->port != PHONE_PORT) goto CHECK_ERR;
		if (pthread_create(&tid, &a, client_thread_phone, arg) != 0) {
			perror("Error: thread.\n");
			goto CHECK_ERR;
		};
		break;
	case TYPE_BED:			/*Client's device is bed.*/
#ifdef DEBUG	
		printf("This is a bed.\n");
#endif
		if(thread_arg->port != BED_PORT) goto CHECK_ERR;
		if (pthread_create(&tid, &a, client_thread_bed, arg) != 0) {
			perror("Error: thread.\n");
			goto CHECK_ERR;
		};

		break;
	}
	return;
	
CHECK_ERR:
	print_device(&device);
	loger(BED, STDERR, "err:get device");
	close(client_fd);
}


void loger(int type, int streamType, int *msg)
{
	FILE* fd;
	char * file;
	char * file_err;
	time_t timep;
	if(type == PHONE)
	{
		file = "./log/Phone_logger";
		file_err = "./log/Phone_err_logger";
	}
	if(type == BED)
	{
		file = "./log/Bed_logger";
		file_err = "./log/Bed_err_logger";
	}
	pthread_mutex_lock(&loger_mutex);
	if(streamType == STDOUT)
		fd = fopen(file, "at+");
	if(streamType == STDERR)
		fd = fopen(file_err, "at+");
	if(fd == NULL)	
	{
		perror("open() failed");
		pthread_mutex_unlock(&loger_mutex);
		return -1;
	}
	
	time (&timep);
	fprintf(fd, "---------------------------------------------------------------\r\n");
	fprintf(fd, "[%s]%s\r\n", asctime(gmtime(&timep)), msg);
	fprintf(fd, "---------------------------------------------------------------\r\n");
	
	fclose(fd);
	pthread_mutex_unlock(&loger_mutex);
}

