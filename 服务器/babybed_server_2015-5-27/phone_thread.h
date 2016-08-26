/*
 * phone_thread.h
 *
 *  Created on: 2016-4-7
 *      Author: train
 */

#ifndef PHONE_THREAD_H_
#define PHONE_THREAD_H_

#include "bed_types.h"

void *client_thread_phone(void *arg);
int get_data_from_mysql(struct DATA_PACKET *cpacket);
int save_command_to_mysql(struct COMMAND_PACKET *cpacket);



#endif /* PHONE_THREAD_H_ */
