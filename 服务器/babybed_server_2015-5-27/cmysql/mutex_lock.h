#ifndef _MUTEX_LOCK_H_
#define _MUTEX_LOCK_H_


void init_mutex(void);
int mysql_lock();
int mysql_unlock();
void mutex_free();
  
#endif