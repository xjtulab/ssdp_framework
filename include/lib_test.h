//
#ifndef LIB_TEST
#define LIB_TEST
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <fcntl.h>              // Flags for open()
#include <sys/stat.h>           // Open() system call
#include <sys/types.h>          // Types for open()
#include <unistd.h>             // Close() system call
#include <string.h>             // Memory setting and copying
#include <getopt.h>             // Option parsing
#include <errno.h>              // Error codes
#include "lib_test.h"
extern "C" {
#include "libmc3s029zesensorinfoget.h"
#include "libmc3s028zecpldcfg.h"
#include "libaxidma.h"          // Interface ot the AXI DMA library
}
using namespace std;

// Converts a tval struct to a double value of the time in seconds
#define TVAL_TO_SEC(tval) \
    (((double)(tval).tv_sec) + (((double)(tval).tv_usec) / 1000000.0))

// Converts a byte (integral) value to mebibytes (floating-point)
#define BYTE_TO_MIB(size) (((double)(size)) / (1024.0 * 1024.0))

// Converts a mebibyte (floating-point) value to bytes (integral)
#define MIB_TO_BYTE(size) ((size_t)((size) * 1024.0 * 1024.0))

struct dma_transfer {
    int input_fd;           // The file descriptor for the input file
    int input_channel;      // The channel used to send the data
    int input_size;         // The amount of data to send
    void *input_buf;        // The buffer to hold the input data
};

int parse_int(char option, char *arg_str, int *data)
{
    int rc;
    rc = sscanf(optarg, "%d", data);
    if (rc < 0) {
        perror("Unable to parse argument");
        return rc;
    } else if (rc != 1) {
        fprintf(stderr, "Error: Unable to parse argument '-%c %s' as an "
                "integer.\n", option, arg_str);
        return -EINVAL;
    }
    return 0;
}

int robust_read(int fd, char *buf, int buf_size)
{
    int bytes_remain, bytes_read;
    int buf_offset;

    // Read out the bytes into the buffer, accounting for EINTR
    bytes_remain = buf_size;
    while (true)
    {
        buf_offset = buf_size - bytes_remain;
        bytes_read = read(fd, buf + buf_offset, bytes_remain);
        bytes_remain = (bytes_read > 0) ? bytes_remain - bytes_read
                                        : bytes_remain;

        /* If we were interrupted by a signal, then repeat the read. Otherwise,
         * if we encountered a different error or reached EOF then stop. */
        if (bytes_read < 0 && bytes_read != -EINTR) {
            return bytes_read;
        } else if (bytes_read == 0) {
            return buf_size - bytes_remain;
        }
    }

    // We should never reach here
    assert(false);
    return -EINVAL;
}

static void print_usage(bool help)
{
    FILE* stream = (help) ? stdout : stderr;

    fprintf(stream, "Usage: axidma_transfer <input path>.\n");
    if (!help) {
        return;
    }

    fprintf(stream, "\t<input path>:\t\tThe path to file to send out over AXI "
            "DMA to the PL fabric. Can be a relative or absolute path.\n");
    return;
}

static int transfer_file(axidma_dev_t dev, struct dma_transfer *trans)
{
    int rc;
    //struct dma_transfer trans_cmd;

    //trans_cmd.input_buf = axidma_malloc(dev, 4);

    // Allocate a buffer for the input file, and read it into the buffer
    trans->input_buf = axidma_malloc(dev, trans->input_size);
    if (trans->input_buf == NULL) {
        fprintf(stderr, "Failed to allocate the input buffer.\n");
        rc = -ENOMEM;
        goto ret;
    }
    rc = robust_read(trans->input_fd, (char*)trans->input_buf, trans->input_size);
    if (rc < 0) {
        perror("Unable to read in input buffer.\n");
        axidma_free(dev, trans->input_buf, trans->input_size);
        return rc;
    }

    /*unsigned int upgrade_cmd = 0xA5A55A5A;
    memcpy(trans_cmd.input_buf, (void *)&upgrade_cmd, 4);

    rc = axidma_oneway_transfer(dev, trans->input_channel, trans_cmd.input_buf, 4, true);
    if (rc < 0) {
        fprintf(stderr, "DMA read write transaction failed.\n");
        goto free_input_buf;
    }

    usleep(1);*/

    // Perform the transfer
    // Perform the main transaction
    rc = axidma_oneway_transfer(dev, trans->input_channel, trans->input_buf,
            trans->input_size, true);
    if (rc < 0) {
        fprintf(stderr, "DMA read write transaction failed.\n");
        goto free_input_buf;
    }

    printf("Upgrade completed!\n");

free_input_buf:
    axidma_free(dev, trans->input_buf, trans->input_size);
ret:
    return rc;
}

int upgrade_test(int argc, char **argv)
{
    int rc;
    char *input_path;
    axidma_dev_t axidma_dev;
    struct stat input_stat;
    struct dma_transfer trans;
    const array_t *tx_chans;

    // Parse the input arguments
    memset(&trans, 0, sizeof(trans));

    if(argc < 2)
    {
	printf("Usage: axidma_transfer <input path>.\n");
	printf("Please set the transfer input file!\n");
	rc = 1;
	goto ret;
    }
    
    input_path = argv[1];

    // Try opening the input and output images
    trans.input_fd = open(input_path, O_RDONLY);
    if (trans.input_fd < 0) {
        perror("Error opening input file");
        rc = 1;
        goto ret;
    }

    // Initialize the AXIDMA device
    axidma_dev = axidma_init("/dev/axidmaupgrade");
    if (axidma_dev == NULL) {
        fprintf(stderr, "Error: Failed to initialize the AXI DMA device.\n");
        rc = 1;
        goto close_input;
    }

    // Get the size of the input file
    if (fstat(trans.input_fd, &input_stat) < 0) {
        perror("Unable to get file statistics");
        rc = 1;
        goto destroy_axidma;
    }

    // If the output size was not specified by the user, set it to the default
    trans.input_size = input_stat.st_size;
    
    // Get the tx and rx channels if they're not already specified
    tx_chans = axidma_get_dma_tx(axidma_dev);
    if (tx_chans->len < 1) {
        fprintf(stderr, "Error: No transmit channels were found.\n");
        rc = -ENODEV;
        goto destroy_axidma;
    }

    /* If the user didn't specify the channels, we assume that the transmit and
     * receive channels are the lowest numbered ones. */
    trans.input_channel = tx_chans->data[0];

    printf("AXI DMA File Transfer Info:\n");
    printf("\tTransmit Channel: %d\n", trans.input_channel);
    printf("\tInput File Size: %d byte\n", trans.input_size);
    printf("\tInput File Size: %.2f MiB\n", BYTE_TO_MIB(trans.input_size));

    // Transfer the file over the AXI DMA
    rc = transfer_file(axidma_dev, &trans);
    rc = (rc < 0) ? -rc : 0;

destroy_axidma:
    axidma_destroy(axidma_dev);
close_input:
    assert(close(trans.input_fd) == 0);
ret:
    return rc;
}

int test_sessor(){
	float voltage_data_tmp=0;
	float temperature_data_tmp=0;
	temperature_data_tmp = get_main_control_board_temperature();
        printf("temperature_data is %f cent\n", temperature_data_tmp);
	
	voltage_data_tmp = get_main_control_board_voltage1_vccaux();
	printf("voltage1_vccaux is %f v\n", voltage_data_tmp);
        
        voltage_data_tmp = get_main_control_board_voltage2_vccbram();
	printf("voltage2_vccbram is %f v\n", voltage_data_tmp);

	voltage_data_tmp = get_main_control_board_voltage3_vccpint();
	printf("voltage3_vccpint is %f v\n", voltage_data_tmp);

	voltage_data_tmp = get_main_control_board_voltage4_vccpaux();
	printf("voltage4_vccpaux is %f v\n", voltage_data_tmp);

	voltage_data_tmp = get_main_control_board_voltage5_vccoddr();
	printf("voltage5_vccoddr is %f v\n", voltage_data_tmp);

	voltage_data_tmp = get_main_control_board_voltage6_vrefp();
	printf("voltage6_vrefp is %f v\n", voltage_data_tmp);

	voltage_data_tmp = get_main_control_board_voltage7_vrefn();
	printf("voltage7_vrefn is %f v\n", voltage_data_tmp);

        if (get_main_control_board_eth0_link_status())
		printf("eth0 link status is ok!\n");
        else
		printf("eth0 link status is not ok!\n");

	if (get_main_control_board_eth1_link_status())
		printf("eth1 link status is ok!\n");
        else
		printf("eth1 link status is not ok!\n");

	if (get_main_control_board_eth2_link_status())
		printf("eth2 link status is ok!\n");
        else
		printf("eth2 link status is not ok!\n");

	if (get_main_control_board_eth3_link_status())
		printf("eth3 link status is ok!\n");
        else
		printf("eth3 link status is not ok!\n");

	if (get_main_control_board_sata_ready_status())
		printf("sata ready status is ok!\n");
        else
		printf("sata ready status is not ok!\n");

	if (get_main_control_board_sata_link_status())
		printf("sata link status is ok!\n");
        else
		printf("sata link status is not ok!\n");

	if (get_main_control_board_cdcm_status())
		printf("cdcm status is ok!\n");
        else
		printf("cdcm status is not ok!\n");

	if (get_main_control_board_srio0_link_status())
		printf("srio0 link status is ok!\n");
        else
		printf("srio0 link status is not ok!\n");

	if (get_main_control_board_srio1_link_status())
		printf("srio1 link status is ok!\n");
        else
		printf("srio1 link status is not ok!\n");

	return 0;
}

int zecpldcfg_test(){
      unsigned int* map_bram_ctrl_address;
    map_bram_ctrl_address = spdcpldop_init();
    
    /****storage board operation interface begin****/
    unsigned int storage_board_slot_no;
    storage_board_slot_no = get_storage_board_slot_no(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_slot_no: 0x%x\n", map_bram_ctrl_address[0], storage_board_slot_no);
    
    unsigned int storage_board_hw_version;
    storage_board_hw_version = get_storage_board_hw_version(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_hw_version: 0x%x\n", map_bram_ctrl_address[1], storage_board_hw_version);

    unsigned int storage_board_flash_sw_version;
    storage_board_flash_sw_version = get_storage_board_flash_sw_version(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_flash_sw_version: 0x%x\n", map_bram_ctrl_address[2], storage_board_flash_sw_version);

    float storage_board_voltage1;
    storage_board_voltage1 = get_storage_board_voltage1(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage1: %f\n", map_bram_ctrl_address[3], storage_board_voltage1);

    float storage_board_voltage2;
    storage_board_voltage2 = get_storage_board_voltage2(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage2: %f\n", map_bram_ctrl_address[4], storage_board_voltage2);

    float storage_board_voltage3;
    storage_board_voltage3 = get_storage_board_voltage3(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage3: %f\n", map_bram_ctrl_address[5], storage_board_voltage3);

    float storage_board_voltage4;
    storage_board_voltage4 = get_storage_board_voltage4(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage4: %f\n", map_bram_ctrl_address[6], storage_board_voltage4);

    float storage_board_voltage5;
    storage_board_voltage5 = get_storage_board_voltage5(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage5: %f\n", map_bram_ctrl_address[7], storage_board_voltage5);

    float storage_board_voltage6;
    storage_board_voltage6 = get_storage_board_voltage6(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage6: %f\n", map_bram_ctrl_address[8], storage_board_voltage6);

    float storage_board_voltage7;
    storage_board_voltage7 = get_storage_board_voltage7(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage7: %f\n", map_bram_ctrl_address[9], storage_board_voltage7);

    float storage_board_voltage8;
    storage_board_voltage8 = get_storage_board_voltage8(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_voltage8: %f\n", map_bram_ctrl_address[10], storage_board_voltage8);

    /*bool is_reset;
    is_reset = storage_board_reset(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_reset_return_value: 0x%x\n", map_bram_ctrl_address[86], is_reset);

    sleep(1);

    bool is_set;
    is_set = storage_board_set(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_set_return_value: 0x%x\n", map_bram_ctrl_address[86], is_set);

    sleep(1);
     
    bool reset_status;
    reset_status = get_storage_board_reset_status(map_bram_ctrl_address);
    printf("address value: 0x%x get_storage_board_reset_status: 0x%x\n", map_bram_ctrl_address[12], reset_status);*/

    float storage_board_temperature1;
    storage_board_temperature1 = get_storage_board_temperature1(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_temperature1: %f\n", map_bram_ctrl_address[15], storage_board_temperature1);

    float storage_board_temperature2;
    storage_board_temperature2 = get_storage_board_temperature2(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_temperature2: %f\n", map_bram_ctrl_address[16], storage_board_temperature2);

    bool storage_board_zynq_srio_link_status;
    storage_board_zynq_srio_link_status = get_storage_board_zynq_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_zynq_srio_link_status: 0x%x\n", map_bram_ctrl_address[18], storage_board_zynq_srio_link_status);

    bool storage_board_fpga1_srio_link_status;
    storage_board_fpga1_srio_link_status = get_storage_board_fpga1_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_fpga1_srio_link_status: 0x%x\n", map_bram_ctrl_address[18], storage_board_fpga1_srio_link_status);

    bool storage_board_fpga2_srio_link_status;
    storage_board_fpga2_srio_link_status = get_storage_board_fpga2_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_fpga2_srio_link_status: 0x%x\n", map_bram_ctrl_address[18], storage_board_fpga2_srio_link_status);

    bool storage_board_zynq_ge_link_status;
    storage_board_zynq_ge_link_status = get_storage_board_zynq_ge_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_zynq_ge_link_status: 0x%x\n", map_bram_ctrl_address[19], storage_board_zynq_ge_link_status);

    bool storage_board_zynq_core_status;
    storage_board_zynq_core_status = get_storage_board_zynq_core_status(map_bram_ctrl_address);
    printf("address value: 0x%x storage_board_zynq_core_status: 0x%x\n", map_bram_ctrl_address[20], storage_board_zynq_core_status);
    /****storage board operation interface end****/

    /****pretreatment board operation interface begin****/
    unsigned int pretreatment_board_slot_no;
    pretreatment_board_slot_no = get_pretreatment_board_slot_no(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_slot_no: 0x%x\n", map_bram_ctrl_address[21], pretreatment_board_slot_no);
    
    unsigned int pretreatment_board_hw_version;
    pretreatment_board_hw_version = get_pretreatment_board_hw_version(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_hw_version: 0x%x\n", map_bram_ctrl_address[22], pretreatment_board_hw_version);

    unsigned int pretreatment_board_flash_sw_version;
    pretreatment_board_flash_sw_version = get_pretreatment_board_flash_sw_version(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_flash_sw_version: 0x%x\n", map_bram_ctrl_address[23], pretreatment_board_flash_sw_version);

    float pretreatment_board_voltage1;
    pretreatment_board_voltage1 = get_pretreatment_board_voltage1(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage1: %f\n", map_bram_ctrl_address[24], pretreatment_board_voltage1);

    float pretreatment_board_voltage2;
    pretreatment_board_voltage2 = get_pretreatment_board_voltage2(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage2: %f\n", map_bram_ctrl_address[25], pretreatment_board_voltage2);

    float pretreatment_board_voltage3;
    pretreatment_board_voltage3 = get_pretreatment_board_voltage3(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage3: %f\n", map_bram_ctrl_address[26], pretreatment_board_voltage3);

    float pretreatment_board_voltage4;
    pretreatment_board_voltage4 = get_pretreatment_board_voltage4(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage4: %f\n", map_bram_ctrl_address[27], pretreatment_board_voltage4);

    float pretreatment_board_voltage5;
    pretreatment_board_voltage5 = get_pretreatment_board_voltage5(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage5: %f\n", map_bram_ctrl_address[28], pretreatment_board_voltage5);

    float pretreatment_board_voltage6;
    pretreatment_board_voltage6 = get_pretreatment_board_voltage6(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage6: %f\n", map_bram_ctrl_address[29], pretreatment_board_voltage6);

    float pretreatment_board_voltage7;
    pretreatment_board_voltage7 = get_pretreatment_board_voltage7(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage7: %f\n", map_bram_ctrl_address[30], pretreatment_board_voltage7);

    float pretreatment_board_voltage8;
    pretreatment_board_voltage8 = get_pretreatment_board_voltage8(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_voltage8: %f\n", map_bram_ctrl_address[31], pretreatment_board_voltage8);

    /*bool is_pretreatment_reset;
    is_pretreatment_reset = pretreatment_board_reset(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_reset_return_value: 0x%x\n", map_bram_ctrl_address[107], is_pretreatment_reset);

    sleep(1);

    bool is_pretreatment_set;
    is_pretreatment_set = pretreatment_board_set(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_set_return_value: 0x%x\n", map_bram_ctrl_address[107], is_pretreatment_set);

    sleep(1);

    bool reset_pretreatment_status;
    reset_pretreatment_status = get_pretreatment_board_reset_status(map_bram_ctrl_address);
    printf("address value: 0x%x get_pretreatment_board_reset_status: 0x%x\n", map_bram_ctrl_address[33], reset_pretreatment_status);*/

    float pretreatment_board_temperature1;
    pretreatment_board_temperature1 = get_pretreatment_board_temperature1(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_temperature1: %f\n", map_bram_ctrl_address[36], pretreatment_board_temperature1);

    float pretreatment_board_temperature2;
    pretreatment_board_temperature2 = get_pretreatment_board_temperature2(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_temperature2: %f\n", map_bram_ctrl_address[37], pretreatment_board_temperature2);

    bool pretreatment_board_fiber1_link_status;
    pretreatment_board_fiber1_link_status = get_pretreatment_board_fiber1_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fiber1_link_status: 0x%x\n", map_bram_ctrl_address[38], pretreatment_board_fiber1_link_status);

    bool pretreatment_board_fiber2_link_status;
    pretreatment_board_fiber2_link_status = get_pretreatment_board_fiber2_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fiber2_link_status: 0x%x\n", map_bram_ctrl_address[38], pretreatment_board_fiber2_link_status);

    bool pretreatment_board_fiber3_link_status;
    pretreatment_board_fiber3_link_status = get_pretreatment_board_fiber3_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fiber3_link_status: 0x%x\n", map_bram_ctrl_address[38], pretreatment_board_fiber3_link_status);

    bool pretreatment_board_fiber4_link_status;
    pretreatment_board_fiber4_link_status = get_pretreatment_board_fiber4_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fiber4_link_status: 0x%x\n", map_bram_ctrl_address[38], pretreatment_board_fiber4_link_status);

    bool pretreatment_board_fpga_srio_link_status;
    pretreatment_board_fpga_srio_link_status = get_pretreatment_board_fpga_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fpga_srio_link_status: 0x%x\n", map_bram_ctrl_address[39], pretreatment_board_fpga_srio_link_status);

    bool pretreatment_board_fpga_ge_link_status;
    pretreatment_board_fpga_ge_link_status = get_pretreatment_board_fpga_ge_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fpga_ge_link_status: 0x%x\n", map_bram_ctrl_address[40], pretreatment_board_fpga_ge_link_status);

    bool pretreatment_board_fpga_core_status;
    pretreatment_board_fpga_core_status = get_pretreatment_board_fpga_core_status(map_bram_ctrl_address);
    printf("address value: 0x%x pretreatment_board_fpga_core_status: 0x%x\n", map_bram_ctrl_address[41], pretreatment_board_fpga_core_status);
    /****pretreatment board operation interface end****/

    /****signal_processing board operation interface begin****/
    unsigned int signal_processing_board_slot_no;
    signal_processing_board_slot_no = get_signal_processing_board_slot_no(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_slot_no: 0x%x\n", map_bram_ctrl_address[42], signal_processing_board_slot_no);
    
    unsigned int signal_processing_board_hw_version;
    signal_processing_board_hw_version = get_signal_processing_board_hw_version(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_hw_version: 0x%x\n", map_bram_ctrl_address[43], signal_processing_board_hw_version);

    unsigned int signal_processing_board_flash_sw_version;
    signal_processing_board_flash_sw_version = get_signal_processing_board_flash_sw_version(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_flash_sw_version: 0x%x\n", map_bram_ctrl_address[44], signal_processing_board_flash_sw_version);

    float signal_processing_board_voltage1;
    signal_processing_board_voltage1 = get_signal_processing_board_voltage1(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage1: %f\n", map_bram_ctrl_address[45], signal_processing_board_voltage1);

    float signal_processing_board_voltage2;
    signal_processing_board_voltage2 = get_signal_processing_board_voltage2(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage2: %f\n", map_bram_ctrl_address[46], signal_processing_board_voltage2);

    float signal_processing_board_voltage3;
    signal_processing_board_voltage3 = get_signal_processing_board_voltage3(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage3: %f\n", map_bram_ctrl_address[47], signal_processing_board_voltage3);

    float signal_processing_board_voltage4;
    signal_processing_board_voltage4 = get_signal_processing_board_voltage4(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage4: %f\n", map_bram_ctrl_address[48], signal_processing_board_voltage4);

    float signal_processing_board_voltage5;
    signal_processing_board_voltage5 = get_signal_processing_board_voltage5(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage5: %f\n", map_bram_ctrl_address[49], signal_processing_board_voltage5);

    float signal_processing_board_voltage6;
    signal_processing_board_voltage6 = get_signal_processing_board_voltage6(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage6: %f\n", map_bram_ctrl_address[50], signal_processing_board_voltage6);

    float signal_processing_board_voltage7;
    signal_processing_board_voltage7 = get_signal_processing_board_voltage7(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage7: %f\n", map_bram_ctrl_address[51], signal_processing_board_voltage7);

    float signal_processing_board_voltage8;
    signal_processing_board_voltage8 = get_signal_processing_board_voltage8(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_voltage8: %f\n", map_bram_ctrl_address[52], signal_processing_board_voltage8);

    /*bool is_signal_processing_reset;
    is_signal_processing_reset = signal_processing_board_reset(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_reset_return_value: 0x%x\n", map_bram_ctrl_address[128], is_signal_processing_reset);

    sleep(1);

    bool is_signal_processing_set;
    is_signal_processing_set = signal_processing_board_set(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_set_return_value: 0x%x\n", map_bram_ctrl_address[128], is_signal_processing_set);

    sleep(1);

    bool reset_signal_processing_status;
    reset_signal_processing_status = get_signal_processing_board_reset_status(map_bram_ctrl_address);
    printf("address value: 0x%x get_signal_processing_board_reset_status: 0x%x\n", map_bram_ctrl_address[54], reset_signal_processing_status);*/

    bool set_spb_boot_from_spi_flash_status;
    set_spb_boot_from_spi_flash_status = set_signal_processing_board_boot_from_spi_flash(map_bram_ctrl_address);
    printf("address value: 0x%x set_signal_processing_board_boot_from_spi_flash_status: 0x%x\n", map_bram_ctrl_address[55], set_spb_boot_from_spi_flash_status);

    bool set_spb_boot_from_network_select_map_status;
    set_spb_boot_from_network_select_map_status = set_signal_processing_board_boot_from_network_select_map(map_bram_ctrl_address);
    printf("address value: 0x%x set_signal_processing_board_boot_from_network_select_map_status: 0x%x\n", map_bram_ctrl_address[55], set_spb_boot_from_network_select_map_status);

    float signal_processing_board_temperature1;
    signal_processing_board_temperature1 = get_signal_processing_board_temperature1(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_temperature1: %f\n", map_bram_ctrl_address[57], signal_processing_board_temperature1);

    float signal_processing_board_temperature2;
    signal_processing_board_temperature2 = get_signal_processing_board_temperature2(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_temperature2: %f\n", map_bram_ctrl_address[58], signal_processing_board_temperature2);

    bool signal_processing_board_dsp1_srio_link_status;
    signal_processing_board_dsp1_srio_link_status = get_signal_processing_board_dsp1_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp1_srio_link_status: 0x%x\n", map_bram_ctrl_address[60], signal_processing_board_dsp1_srio_link_status);

    bool signal_processing_board_dsp2_srio_link_status;
    signal_processing_board_dsp2_srio_link_status = get_signal_processing_board_dsp2_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp2_srio_link_status: 0x%x\n", map_bram_ctrl_address[60], signal_processing_board_dsp2_srio_link_status);

    bool signal_processing_board_dsp3_srio_link_status;
    signal_processing_board_dsp3_srio_link_status = get_signal_processing_board_dsp3_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp3_srio_link_status: 0x%x\n", map_bram_ctrl_address[60], signal_processing_board_dsp3_srio_link_status);

    bool signal_processing_board_dsp4_srio_link_status;
    signal_processing_board_dsp4_srio_link_status = get_signal_processing_board_dsp4_srio_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp4_srio_link_status: 0x%x\n", map_bram_ctrl_address[60], signal_processing_board_dsp4_srio_link_status);

    bool signal_processing_board_dsp1_ge_link_status;
    signal_processing_board_dsp1_ge_link_status = get_signal_processing_board_dsp1_ge_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp1_ge_link_status: 0x%x\n", map_bram_ctrl_address[61], signal_processing_board_dsp1_ge_link_status);

    bool signal_processing_board_dsp2_ge_link_status;
    signal_processing_board_dsp2_ge_link_status = get_signal_processing_board_dsp2_ge_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp2_ge_link_status: 0x%x\n", map_bram_ctrl_address[61], signal_processing_board_dsp2_ge_link_status);

    bool signal_processing_board_dsp3_ge_link_status;
    signal_processing_board_dsp3_ge_link_status = get_signal_processing_board_dsp3_ge_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp3_ge_link_status: 0x%x\n", map_bram_ctrl_address[61], signal_processing_board_dsp3_ge_link_status);

    bool signal_processing_board_dsp4_ge_link_status;
    signal_processing_board_dsp4_ge_link_status = get_signal_processing_board_dsp4_ge_link_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp4_ge_link_status: 0x%x\n", map_bram_ctrl_address[61], signal_processing_board_dsp4_ge_link_status);

    bool signal_processing_board_dsp1_core_status;
    signal_processing_board_dsp1_core_status = get_signal_processing_board_dsp1_core_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp1_core_status: 0x%x\n", map_bram_ctrl_address[62], signal_processing_board_dsp1_core_status);

    bool signal_processing_board_dsp2_core_status;
    signal_processing_board_dsp2_core_status = get_signal_processing_board_dsp2_core_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp2_core_status: 0x%x\n", map_bram_ctrl_address[62], signal_processing_board_dsp2_core_status);

    bool signal_processing_board_dsp3_core_status;
    signal_processing_board_dsp3_core_status = get_signal_processing_board_dsp3_core_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp3_core_status: 0x%x\n", map_bram_ctrl_address[62], signal_processing_board_dsp3_core_status);

    bool signal_processing_board_dsp4_core_status;
    signal_processing_board_dsp4_core_status = get_signal_processing_board_dsp4_core_status(map_bram_ctrl_address);
    printf("address value: 0x%x signal_processing_board_dsp4_core_status: 0x%x\n", map_bram_ctrl_address[62], signal_processing_board_dsp4_core_status);

    /****pretreatment board operation interface end****/

    spdcpldop_release();

    return 0;
}
#endif