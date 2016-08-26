#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bed_types.h"
#include "bed_server.h"
#include "pthread.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include    <fcntl.h>  
#include "phone_thread.h"
#include "cmysql/mutex_lock.h"
#include <signal.h>

int server_run();
int fork_phone();
int fork_bed();

int run=1;
pid_t bed_pid;
pid_t phone_pid;

void handle_sig_child()//捕捉SIGCHLD信号后的处理fun
{
	 int ret; 
	 pid_t pid_c;
	 pid_c=wait(&ret);//使进程睡眠，直到它的一个子进程退出时唤醒
	 //pid_t wait(int *status) 返回值是退出的子进程的pid , status是子进程退出的状态信息
	 //还有一个是pid_t waitpid(pid_t pid,int *status,int options)
	 printf("Child proess PID = %d return %d.\n",pid_c,ret);
	 if(pid_c == phone_pid)
		fork_phone();
	 if(pid_c == bed_pid)
		 fork_bed();
	 
}

int main(int argc, char **argv) {
	// if(argc <=1) {
		// perror("Usage: bed_server port");
		// return 0;
	// }
	// int port = atoi(argv[1]);
	pid_t pid;
	init_mutex();  //创建共享的mutex
	// printf("prot=%d, sid = %d\n", port, sockfd);

	/*wait for server*/
	printf("Server is runing, input 'q' for ending.\n");
	
	if((pid = fork())>0)
		exit(0);
	else if(pid<0)
	{
		perror("fail to fork");
		exit(-1);
	}
	setsid(); 
	if(pid=fork()) 
      exit(0);
	server_run();


	/*block, wait for ending*/
	char c;
	while((c = getchar()) != 'q')
	{}

	/*server ending*/
	wait(NULL); 
	mutex_free();
	
	printf("server end\n");
	return 0;
}


void *_server_start_thread(void *arg)
{
	Targ *a = (Targ *)arg;
	//int sockfd = a->sockfd;

	while(run) {
			socket_accept(a->sockfd, a->port);	//accept
	}
	return 0;
}



int fork_phone()
{
	phone_pid = fork();
	if(phone_pid == 0)
	{
		printf("start phone\n");
			/*get socket descriptor.*/
		int sockfd = socket_initialize(PHONE_PORT);
		Targ arg;
		arg.sockfd = sockfd;
		arg.port = PHONE_PORT;
		_server_start_thread((void *)&arg);
		close(sockfd);
	}
	return phone_pid;
}

int fork_bed()
{
	bed_pid = fork();
	if(bed_pid == 0)
	{
		printf("start bed\n");
		/*get socket descriptor.*/
		int sockfd2 = socket_initialize(BED_PORT);
		Targ arg;
		arg.sockfd = sockfd2;
		arg.port = BED_PORT;
		_server_start_thread((void *)&arg);
		close(sockfd2);
	}
	return bed_pid;
}

int server_run()
{
	
	signal(SIGCHLD, handle_sig_child);//捕捉SIGCHLD信号,并调取handle_sig_child()进行处理 ,(信号值，处理func)

	if(fork_phone()>0)
	{
		fork_bed();
	}

	return 0;
}

int server_end(int sockfd, int sockfd2) {
	run = 0;
	close(sockfd);
	close(sockfd2);
	return 0;
}




