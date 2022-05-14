/*
 * ssdp_messageQ.h
 *
 *  Created on: 2021Äê5ÔÂ7ÈÕ
 *      Author: fh
 */

#ifndef SSDP_MESSAGEQ_H_
#define SSDP_MESSAGEQ_H_

#include "HelloWorld.h"
#include <uxr/client/client.h>
#include <ucdr/microcdr.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <stdbool.h>

#define MAXSIZE            20
#define STREAM_HISTORY     8
#define BUFFER_SIZE        512*STREAM_HISTORY



bool ssdp_init_udp(char *ip, char *port, char *topic_reader, char* topic_writer, uint32_t reader_session_key, uint32_t writer_session_key);
//bool ssdp_init_srio();
bool ssdp_pub_topic(char *topic);
bool ssdp_sub_topic(char *topic);
int ssdp_udp_send_data(char *reVal);
//bool ssdp_srio_send_data();
int ssdp_udp_recv_data(bool start_t, char *buf, int len);
//bool ssdp_srio_recv_data();

#endif /* SSDP_MESSAGEQ_H_ */
