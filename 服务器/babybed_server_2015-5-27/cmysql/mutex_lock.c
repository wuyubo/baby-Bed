
#include "mutex_lock.h"
#include    <stdio.h>  
#include    <stdlib.h>  
#include    <unistd.h>  
#include    <fcntl.h>  
#include    <sys/mman.h>  
#include    <pthread.h>  

pthread_mutex_t* g_mutex;  

//创建共享的mutex  
void init_mutex(void)  
{  
    int ret;  
    //g_mutex一定要是进程间可以共享的，否则无法达到进程间互斥  
    g_mutex=(pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);  
    if( MAP_FAILED==g_mutex )  
    {  
        perror("mmap");  
        exit(1);  
    }  
      
    //设置attr的属性  
    pthread_mutexattr_t attr;  
    pthread_mutexattr_init(&attr);  
    //一定要设置为PTHREAD_PROCESS_SHARED  
    ret=pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);  
    if( ret!=0 )  
    {  
        perror("init_mutex pthread_mutexattr_setpshared");  
        exit(1);  
    }  
    pthread_mutex_init(g_mutex, &attr);  
} 


int mysql_lock()
{
	int ret;
	ret=pthread_mutex_lock(g_mutex); //lock 
	if( ret!=0 )  
	{  
	    perror("father pthread_mutex_lock");  
	    return -1;
	}  
	return ret;
}


int mysql_unlock()
{
	int ret = pthread_mutex_unlock(g_mutex);    //unlock 
	if( ret!=0 )  
	{  
	    perror("father pthread_mutex_unlock");  
	    return -1;
	}
	return ret;
}

void mutex_free()
{
	munmap(g_mutex, sizeof(pthread_mutex_t));
}