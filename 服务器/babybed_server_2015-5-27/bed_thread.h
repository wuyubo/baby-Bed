/*
 * bed_thread.h
 *
 *  Created on: 2016-4-12
 *      Author: train
 */

#ifndef BED_THREAD_H_
#define BED_THREAD_H_
#include "bed_types.h"

void *client_thread_bed(void *arg);
int get_command_from_mysql(struct COMMAND_PACKET *cmd);
int save_data_to_mysql(struct DATA_PACKET *cpacket);
int update_status(struct DATA_PACKET *cpacket);



#endif /* BED_THREAD_H_ */
