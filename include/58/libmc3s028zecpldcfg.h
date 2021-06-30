/**
 * @file libmc3s028zecpldcfg.h
 * @date 20210322
 * @author zhujw
 *
 * This file defines the interface to operate the signal processing board cpld
 * library.
 **/

#ifndef LIBMC3S028ZECPLDOP_H_
#define LIBMC3S028ZECPLDOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned int* spdcpldop_init();
void spdcpldop_release();

/****storage board operation interface begin****/
unsigned int get_storage_board_slot_no(unsigned int* base_address);
unsigned int get_storage_board_hw_version(unsigned int* base_address);
unsigned int get_storage_board_flash_sw_version(unsigned int* base_address);
float get_storage_board_voltage1(unsigned int* base_address);
float get_storage_board_voltage2(unsigned int* base_address);
float get_storage_board_voltage3(unsigned int* base_address);
float get_storage_board_voltage4(unsigned int* base_address);
float get_storage_board_voltage5(unsigned int* base_address);
float get_storage_board_voltage6(unsigned int* base_address);
float get_storage_board_voltage7(unsigned int* base_address);
float get_storage_board_voltage8(unsigned int* base_address);
bool storage_board_reset(unsigned int* base_address);
bool storage_board_set(unsigned int* base_address);
bool get_storage_board_reset_status(unsigned int* base_address);
float get_storage_board_temperature1(unsigned int* base_address);
float get_storage_board_temperature2(unsigned int* base_address);
bool get_storage_board_zynq_srio_link_status(unsigned int* base_address);
bool get_storage_board_fpga1_srio_link_status(unsigned int* base_address);
bool get_storage_board_fpga2_srio_link_status(unsigned int* base_address);
bool get_storage_board_zynq_ge_link_status(unsigned int* base_address);
bool get_storage_board_zynq_core_status(unsigned int* base_address);
/****storage board operation interface end****/

/****pretreatment board operation interface begin****/
int get_pretreatment_board_slot_no(unsigned int* base_address);
unsigned int get_pretreatment_board_hw_version(unsigned int* base_address);
int get_pretreatment_board_flash_sw_version(unsigned int* base_address);
float get_pretreatment_board_voltage1(unsigned int* base_address);
float get_pretreatment_board_voltage2(unsigned int* base_address);
float get_pretreatment_board_voltage3(unsigned int* base_address);
float get_pretreatment_board_voltage4(unsigned int* base_address);
float get_pretreatment_board_voltage5(unsigned int* base_address);
float get_pretreatment_board_voltage6(unsigned int* base_address);
float get_pretreatment_board_voltage7(unsigned int* base_address);
float get_pretreatment_board_voltage8(unsigned int* base_address);
bool pretreatment_board_reset(unsigned int* base_address);
bool pretreatment_board_set(unsigned int* base_address);
bool get_pretreatment_board_reset_status(unsigned int* base_address);
bool get_pretreatment_board_upgrade_complete_status(unsigned int* base_address);
int get_pretreatment_board_upgrade_complete_time(unsigned int* base_address);
float get_pretreatment_board_temperature1(unsigned int* base_address);
float get_pretreatment_board_temperature2(unsigned int* base_address);
bool get_pretreatment_board_fiber1_link_status(unsigned int* base_address);
bool get_pretreatment_board_fiber2_link_status(unsigned int* base_address);
bool get_pretreatment_board_fiber3_link_status(unsigned int* base_address);
bool get_pretreatment_board_fiber4_link_status(unsigned int* base_address);
bool get_pretreatment_board_fpga_srio_link_status(unsigned int* base_address);
bool get_pretreatment_board_fpga_ge_link_status(unsigned int* base_address);
bool get_pretreatment_board_fpga_core_status(unsigned int* base_address);
/****pretreatment board operation interface end*****/

/****signal processing board operation interface begin****/
int get_signal_processing_board_slot_no(unsigned int* base_address);
unsigned int get_signal_processing_board_hw_version(unsigned int* base_address);
int get_signal_processing_board_flash_sw_version(unsigned int* base_address);
float get_signal_processing_board_voltage1(unsigned int* base_address);
float get_signal_processing_board_voltage2(unsigned int* base_address);
float get_signal_processing_board_voltage3(unsigned int* base_address);
float get_signal_processing_board_voltage4(unsigned int* base_address);
float get_signal_processing_board_voltage5(unsigned int* base_address);
float get_signal_processing_board_voltage6(unsigned int* base_address);
float get_signal_processing_board_voltage7(unsigned int* base_address);
float get_signal_processing_board_voltage8(unsigned int* base_address);
bool signal_processing_board_reset(unsigned int* base_address);
bool signal_processing_board_set(unsigned int* base_address);
bool get_signal_processing_board_reset_status(unsigned int* base_address);
bool set_signal_processing_board_boot_from_spi_flash(unsigned int* base_address);
bool set_signal_processing_board_boot_from_network_select_map(unsigned int* base_address);
float get_signal_processing_board_temperature1(unsigned int* base_address);
float get_signal_processing_board_temperature2(unsigned int* base_address);
bool get_signal_processing_board_dsp1_srio_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp2_srio_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp3_srio_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp4_srio_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp1_ge_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp2_ge_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp3_ge_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp4_ge_link_status(unsigned int* base_address);
bool get_signal_processing_board_dsp1_core_status(unsigned int* base_address);
bool get_signal_processing_board_dsp2_core_status(unsigned int* base_address);
bool get_signal_processing_board_dsp3_core_status(unsigned int* base_address);
bool get_signal_processing_board_dsp4_core_status(unsigned int* base_address);
bool get_signal_processing_board_dsp1_upgrade_complete_status(unsigned int* base_address);
bool get_signal_processing_board_dsp2_upgrade_complete_status(unsigned int* base_address);
bool get_signal_processing_board_dsp3_upgrade_complete_status(unsigned int* base_address);
bool get_signal_processing_board_dsp4_upgrade_complete_status(unsigned int* base_address);
int get_signal_processing_board_dsp1_upgrade_complete_time(unsigned int* base_address);
int get_signal_processing_board_dsp2_upgrade_complete_time(unsigned int* base_address);
int get_signal_processing_board_dsp3_upgrade_complete_time(unsigned int* base_address);
int get_signal_processing_board_dsp4_upgrade_complete_time(unsigned int* base_address);
/****signal processing board operation interface end*****/


#endif /* LIBMC3S028ZECPLDOP_H_ */

