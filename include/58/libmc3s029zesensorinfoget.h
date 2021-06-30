/**
 * @file libmc3s029zesensorinfoget.h
 * @date 20210322
 * @author zhujw
 *
 * This file defines the interface to get the main control board sensor information
 * library.
 **/

#ifndef LIBMC3S029ZESENSORINFOGET_H_
#define LIBMC3S029ZESENSORINFOGET_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

float get_main_control_board_temperature();
float get_main_control_board_voltage1_vccaux();
float get_main_control_board_voltage2_vccbram();
float get_main_control_board_voltage3_vccpint();
float get_main_control_board_voltage4_vccpaux();
float get_main_control_board_voltage5_vccoddr();
float get_main_control_board_voltage6_vrefp();
float get_main_control_board_voltage7_vrefn();
bool get_main_control_board_eth0_link_status();
bool get_main_control_board_eth1_link_status();
bool get_main_control_board_eth2_link_status();
bool get_main_control_board_eth3_link_status();
bool get_main_control_board_sata_ready_status();
bool get_main_control_board_sata_link_status();
bool get_main_control_board_cdcm_status();
bool get_main_control_board_srio0_link_status();
bool get_main_control_board_srio1_link_status();
unsigned long get_main_control_board_total_memory();
unsigned long get_main_control_board_free_memory();
int get_main_control_board_cpu_usage();

#endif /* LIBMC3S029ZESENSORINFOGET_H_ */
