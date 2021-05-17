#include "devices/DeviceFPGA.h"
#ifdef ARM_BUILD
    extern "C"{
        #include "58/libmc3s028zecpldcfg.h"
    }
#endif

#ifdef ARM_BUILD
extern "C"{
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

    #include "58/libaxidma.h"          // Interface ot the AXI DMA library

    //设备upgrade用
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
}
#endif

//重载函数实现
DeviceFPGA::DeviceFPGA(string name, SSDP_HandleID id): DeviceBase(name, id){
    SSDP_Result DEV_OK =  DEV_Check();
    if(DEV_OK == SSDP_OK){
        cout<<"Device Check ok"<<endl;
        state = DEVICE_READY;
    }else{
        cout<<"Warning: device not ready"<<endl;
    }
}

std::string DeviceFPGA::DEV_Status_Qeury(){
    float pretreatment_board_voltage1 = 0;
    float pretreatment_board_temperature1 = 0;
    #ifdef ARM_BUILD
        unsigned int* map_bram_ctrl_address;
        map_bram_ctrl_address = spdcpldop_init();
        pretreatment_board_voltage1 = get_pretreatment_board_voltage1(map_bram_ctrl_address);
        pretreatment_board_temperature1 = get_pretreatment_board_temperature1(map_bram_ctrl_address);
        spdcpldop_release();
    #endif
    std::string res = "<device>"
		                "<Id>fpga</Id>"
		                "<vol>"+std::to_string(pretreatment_board_voltage1)+"</vol>"
                        "<temp>"+std::to_string(pretreatment_board_temperature1)+"</temp>"
                       "</device>";
    return res;
}

SSDP_Result DeviceFPGA::DEV_Check(){
    SSDP_Result res = SSDP_OK;
    #ifdef ARM_BUILD
        unsigned int* map_bram_ctrl_address;
        map_bram_ctrl_address = spdcpldop_init();
        if(get_pretreatment_board_fiber1_link_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fiber1_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fiber1_link_status is not ok"<<endl;
        }
        if( get_pretreatment_board_fiber2_link_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fiber2_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fiber2_link_status is not ok"<<endl;
        }
        if( get_pretreatment_board_fiber3_link_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fiber3_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fiber3_link_status is not ok"<<endl;
        }
        if( get_pretreatment_board_fiber4_link_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fiber4_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fiber4_link_status is not ok"<<endl;
        }
        if(get_pretreatment_board_fpga_srio_link_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fpga_srio_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fpga_srio_link_status is not ok"<<endl;
        }
        if(get_pretreatment_board_fpga_ge_link_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fpga_ge_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fpga_ge_link_status is not ok"<<endl;
        }
        if(get_pretreatment_board_fpga_core_status(map_bram_ctrl_address)){
            cout<<"pretreatment_board_fpga_core_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"pretreatment_board_fpga_core_status is not ok"<<endl;
        }
        spdcpldop_release();
    #endif
    return res;
}

SSDP_Result DeviceFPGA::DEV_Load(string filename){
    SSDP_Result rc = SSDP_OK;
    #ifdef ARM_BUILD
    char *input_path;
    axidma_dev_t axidma_dev;
    struct stat input_stat;
    struct dma_transfer trans;
    const array_t *tx_chans;
    
    // Parse the input arguments
    memset(&trans, 0, sizeof(trans));

    input_path = (char*)filename.c_str();

    // Try opening the input and output images
    trans.input_fd = open(input_path, O_RDONLY);
    if (trans.input_fd < 0) {
        perror("Error opening input file");
        rc = SSDP_ERROR;
        goto ret;
    }

    // Initialize the AXIDMA device
    axidma_dev = axidma_init("/dev/axidmaupgrade");
    if (axidma_dev == NULL) {
        fprintf(stderr, "Error: Failed to initialize the AXI DMA device.\n");
        rc = SSDP_ERROR;
        goto close_input;
    }

    // Get the size of the input file
    if (fstat(trans.input_fd, &input_stat) < 0) {
        perror("Unable to get file statistics");
        rc = SSDP_ERROR;
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
    #endif
ret:
    return rc;
}