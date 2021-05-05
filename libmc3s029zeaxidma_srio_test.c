/**
 * @file axidma_benchmark.c
 * @date Thursday, November 26, 2015 at 10:29:26 PM EST
 * @author Brandon Perez (bmperez)
 * @author Jared Choi (jaewonch)
 *
 * This is a simple program that benchmarks the AXI DMA transfer rate for
 * whatever logic is sitting on the PL fabric. It sends some data out over the
 * PL fabric via AXI DMA to whatever is sitting there, and waits to receive
 * some data back from the PL fabric.
 *
 * The program first runs a single transfer to verify that the DMA works
 * properly, then profiles the DMA engine. The program sends out a specific
 * transfer size, and gets back a potentially different receive size. It runs
 * the a given number of times to calculate the performance statistics. All of
 * these options are configurable from the command line.
 *
 * NOTE: This program assumes that there are only two DMA channels being used by
 * the PL fabric, one that consumes data and sends it to the PL fabric logic,
 * and another that sends the output of the PL fabric back to memory. If you
 * have additional DMA channels, you will need to modify the program. This
 * program will work with the AXI DMA/VDMA loopback examples (where the S2MM and
 * MM2S ports are simply connected to one another).
 *
 * @bug No known bugs.
 **/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>             // Strlen function
#include <termios.h>
#include <fcntl.h>              // Flags for open()
#include <sys/stat.h>           // Open() system call
#include <sys/types.h>          // Types for open()
#include <sys/mman.h>           // Mmap system call
#include <sys/ioctl.h>          // IOCTL system call
#include <unistd.h>             // Close() system call
#include <sys/time.h>           // Timing functions and definitions
#include <getopt.h>             // Option parsing
#include <errno.h>              // Error codes
#include "libaxidma.h"          // Interface to the AXI DMA

// Converts a tval struct to a double value of the time in seconds
#define TVAL_TO_SEC(tval) \
    (((double)(tval).tv_sec) + (((double)(tval).tv_usec) / 1000000.0))

// Converts a byte (integral) value to mebibytes (floating-point)
#define BYTE_TO_MIB(size) (((double)(size)) / (1024.0 * 1024.0))

// Converts a mebibyte (floating-point) value to bytes (integral)
#define MIB_TO_BYTE(size) ((size_t)((size) * 1024.0 * 1024.0))

#define DMA_BURST_SIZE          (256+8)
#define HELLO_AND_PLUS_PACKEAGE_SIZE (8+1)


#define DEFAULT_TRANSFER_SIZE       (256+8)
#define DEFAULT_NUM_TRANSFERS       100

bool bLoop = false;
bool bRead = false;
bool bDoorBell = false;
int num_transfers = 1;

int parse_int0x(char option, char *arg_str, int *data)
{
	int rc;
	rc = sscanf(optarg, "%x", data);
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

// Prints the usage for this program
static void print_usage(bool help)
{
    	FILE* stream = (help) ? stdout : stderr;
    	double default_size;

    	fprintf(stream, "Usage: libmc3s029zeaxidma_srio_test [-l <Enable loopback>]"
            "[-o <Enable Read>] [-b <Enable transmit DoorBell Data>] "
            "[-c <SRIO Source ID>] [-d <SRIO Destination ID)>] "
            "[-t <Tx transfer size (bytes)>] [-r <Rx transfer size (bytes)>] "
            "[-n <number transfers>]\n");
    	if (!help) {
        	return;
    	}

    	default_size = BYTE_TO_MIB(DEFAULT_TRANSFER_SIZE);
	fprintf(stream, "\t-l:\t\t\t\tEnable loopback.\n");
    	fprintf(stream, "\t-o:\t\t\t\tEnable Read.\n");
    	fprintf(stream, "\t-b:\t\t\tEnable transmit DoorBell Data.\n");
    	fprintf(stream, "\t-c <SRIO Source ID>:\t\t\tSRIO Source ID,you must set it.\n");
    	fprintf(stream, "\t-d <SRIO Destination ID>:\tSRIO Destination ID,you must set it.\n");
    	fprintf(stream, "\t-t <Tx transfer size (bytes)>:\tTx transfer size (bytes). Default is %d "
            "bytes.\n", DEFAULT_TRANSFER_SIZE);
	fprintf(stream, "\t-t <Rx transfer size (bytes)>:\tRx transfer size (bytes). Default is %d "
            "bytes.\n", DEFAULT_TRANSFER_SIZE);
    	fprintf(stream, "\t-n <number transfers>:\t\t\tThe number of DMA transfers "
            "to perform to do the benchmark. Default is %d transfers.\n",
            DEFAULT_NUM_TRANSFERS);
    	return;
}

/* Parses the command line arguments overriding the default transfer sizes,
 * and number of transfer to use for the benchmark if specified. */
static int parse_args(int argc, char **argv,
        int *nSourceID, int *nDestID,
        size_t *tx_size,  size_t *rx_size,
        int *num_transfers)
{
	double double_arg;
    	int int_arg;
    	char option;
    	bool tx_frame_specified, rx_frame_specified;

    	// Set the default data size and number of transfers
    	*tx_size = DEFAULT_TRANSFER_SIZE;
    	*rx_size = DEFAULT_TRANSFER_SIZE;
    	*num_transfers = DEFAULT_NUM_TRANSFERS;

    	while ((option = getopt(argc, argv, "lobc:d:t:r:n:h")) != (char)-1)
    	{
        	switch (option)
        	{
            		case 'l':
                	bLoop = true;
                	break;

            		case 'o':
                	bRead = true;
                	break;

            		case 'b':
                	bDoorBell = true;
                	break;

            		case 'c':
                	if (parse_int0x(option, optarg, &int_arg) < 0) 
                	{
                    		print_usage(false);
                    		return -EINVAL;
                	}
                	*nSourceID = int_arg;
                	break;

            		case 'd':
                	if (parse_int0x(option, optarg, &int_arg) < 0) 
                	{
                    		print_usage(false);
                    		return -EINVAL;
                	}
                	*nDestID = int_arg;
                	break;

            		// Parse the transmit transfer size argument
            		case 't':
                	if (parse_int(option, optarg, &int_arg) < 0) {
                    		print_usage(false);
                    		return -EINVAL;
                	}
                	*tx_size = int_arg;
                	break;

            		// Parse the receive transfer size argument
            		case 'r':
                	if (parse_int(option, optarg, &int_arg) < 0) {
                    		print_usage(false);
                    		return -EINVAL;
                	}
                	*rx_size = int_arg;
                	break;

            		// Parse the number of transfers argument
            		case 'n':
                	if (parse_int(option, optarg, &int_arg) < 0) {
                    		print_usage(false);
                    		return -EINVAL;
                	}
                	*num_transfers = int_arg;
                	break;

            		// Print detailed usage message
            		case 'h':
                		print_usage(true);
                		exit(0);

            		default:
                		print_usage(false);
                	return -EINVAL;
        	}
    	}

    	if ((*tx_size == DEFAULT_TRANSFER_SIZE) ^ (*rx_size == DEFAULT_TRANSFER_SIZE)) {
        	fprintf(stderr, "Error: If one of -i/-b or -o/-s is specified, then "
                	"both most be.\n");
        	return -EINVAL;
   	}

    	return 0;
}


static void init_data(char *tx_buf, char *rx_buf, size_t tx_buf_size, size_t rx_buf_size, size_t noffset)
{
	int Count = 8;
  	if(tx_buf_size <= HELLO_AND_PLUS_PACKEAGE_SIZE || tx_buf_size >DMA_BURST_SIZE || 
     		rx_buf_size <= HELLO_AND_PLUS_PACKEAGE_SIZE || rx_buf_size>DMA_BURST_SIZE)
	{
	   	fprintf(stderr, "Expected send or rev data size > 9 and <= 256\n");
	   	return;
	}

		
	 /**< Frame the SRIO Write-Stream Packet in the Memory 
	 * The Packet-format used here is HELLO Packet format
	 * More details look into pg 3.1 version:73 page(HELLO PACKET FORMAT).
	 */
	*(unsigned int *)(tx_buf + 0x00) = 0x93900000/*0x50000600*/;
		/* Lower word of the HELLO Packet */
	*(unsigned int *)(tx_buf + 0x04) = 0x00602F74; /*0x08602F74*/
		/* Upper word of the HELLO packet */
		

        *(unsigned char *)(tx_buf + Count) = tx_buf_size-8-1;
        Count = Count + 1;
	while(Count < tx_buf_size) 
	{
		*(unsigned char *)(tx_buf + Count) = (Count-9 + noffset)&0xff;
		 Count += 1;
	}

	memset(rx_buf, 0, rx_buf_size);
	return;
}



static void init_doorbell_data(char *tx_buf, size_t tx_buf_size)
{
  	int Count = 8;
  	if(tx_buf_size <= HELLO_AND_PLUS_PACKEAGE_SIZE || tx_buf_size >DMA_BURST_SIZE )
	{
	   fprintf(stderr, "Expected send or rev data size > 9 and <= 256\n");
	   return;
	}

	/**< Frame the SRIO Write-Stream Packet in the Memory 
	 * The Packet-format used here is HELLO Packet format
	 * More details look into pg 3.1 version:73 page(HELLO PACKET FORMAT).
	 */

        //memset(tx_buf, 0, tx_buf_size);
	*(unsigned int *)(tx_buf + 0x00) = 0x00020000/*0x50000600*/;
		/* Lower word of the HELLO Packet */
	*(unsigned int *)(tx_buf + 0x04) = 0x00A02F74;
		/* Upper word of the HELLO packet */
		
	return;
}


static int verify_data(char *tx_buf, size_t tx_buf_size,
                       char *rx_buf, size_t rx_buf_size)
{
        int Count = HELLO_AND_PLUS_PACKEAGE_SIZE;
	while(Count < rx_buf_size) 
	{

                if(*(tx_buf+Count) != *(rx_buf+Count))
                {
                	printf("\t read data error\n\n");
                	return -1;
                }
		Count += 1;
	}
        return 0;
}

static int single_transfer_test(axidma_dev_t dev,
	int nSourceId, int nDestId,
	int tx_channel,int rx_channel,
	void *tx_buf,int tx_size, 
	void *rx_buf, int rx_size)
{
    	int rc;
    	int i = 0;
    	int j = 0;
    	int value = 0;
    
    	int nDeviceId = ((nSourceId<<16)&0xffff0000) | (nDestId&0xffff);
    
    	write_device_id(nDeviceId, 0);
    	for(i = 0; i < num_transfers; i++)
    	{
    		init_data(tx_buf, rx_buf, tx_size, rx_size, i);
    		rc = axidma_oneway_transfer( dev, tx_channel, tx_buf, tx_size, true );
    		if(rc==0)
    		{
        		printf("\toneway_transfer write finished\n");
    		}
    		else
    		{
        		printf("\toneway_transfer write failed\n\n");
    		}
    
    		if(bDoorBell)
    		{
    	 		init_doorbell_data(tx_buf, tx_size);  //use rx_buf
   	 		rc = axidma_oneway_transfer( dev, tx_channel, tx_buf, 8, true ); //doolbell info 0x0002
   	 		if(rc==0)
   	 		{
  	     			printf("\toneway_transfer write doorbell finished\n\n");
 	 		}
  	 		else
  	 		{
  	     			printf("\toneway_transfer write doorbell failed\n\n");
  	 		}
    		}

     		if(bLoop)
     		{

   	 		if(bRead)
 	 		{
   	        		rc = axidma_oneway_transfer( dev, rx_channel, rx_buf, rx_size, true);
    				if(rc==0)
    				{
                   			if(verify_data(tx_buf,tx_size,rx_buf, rx_size))
                   			{
                        			return -1;
                   			}            
   	 			}
   				else
   				{
       		   			printf("\toneway_transfer read failed\n\n");
  				}
        		 }
    	 	}
	}
    	//Verify that the data in the buffer changed
    	return 0;
}

/*----------------------------------------------------------------------------
 * Main Function
 *----------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
    	int rc;
    	int nSourceId,nDestId;
    	int tx_channel=-1;
    	int rx_channel=-1;
    	size_t tx_size, rx_size;
    	char *tx_buf, *rx_buf;
    	axidma_dev_t axidma_dev;
    	const array_t *tx_chans, *rx_chans;

    	// Check if the user overrided the default transfer size and number
    	if (parse_args(argc, argv, &nSourceId,&nDestId, &tx_size, &rx_size, &num_transfers) < 0) {
        	rc = 1;
        	goto ret;
    	}
    	printf("Mc3s029ze axi dma srio lib test:\n");
    	printf("\tTransmit Buffer Size: %d Byte\n", tx_size);
    	printf("\tReceive Buffer Size: %d Byte MiB\n", rx_size);
    	printf("\tNumber of DMA Transfers: %d transfers\n\n", num_transfers);

    	// Initialize the AXI DMA device
    	axidma_dev = axidma_init("/dev/axidma0");
    	if (axidma_dev == NULL) {
        	fprintf(stderr, "Failed to initialize the AXI DMA device.\n");
        	rc = 1;
        	goto ret;
    	}

    	// Map memory regions for the transmit and receive buffers
    	tx_buf = axidma_malloc(axidma_dev, tx_size);
    	if (tx_buf == NULL) {
        	perror("Unable to allocate transmit buffer from the AXI DMA device.");
        	rc = -1;
        	goto destroy_axidma;
    	}
    	rx_buf = axidma_malloc(axidma_dev, rx_size);
    	if (rx_buf == NULL) {
        	perror("Unable to allocate receive buffer from the AXI DMA device");
        	rc = -1;
        	goto free_tx_buf;
    	}

    	// Get all the transmit and receive channels
        tx_chans = axidma_get_dma_tx(axidma_dev);
        rx_chans = axidma_get_dma_rx(axidma_dev);

    
    	if (tx_chans->len < 1) {
        	fprintf(stderr, "Error: No transmit channels were found.\n");
        	rc = -ENODEV;
        	goto free_rx_buf;
    	}
    	if (rx_chans->len < 1) {
        	fprintf(stderr, "Error: No receive channels were found.\n");
        	rc = -ENODEV;
        	goto free_rx_buf;
    	}

    	/* If the user didn't specify the channels, we assume that the transmit and
     	* receive channels are the lowest numbered ones. */
    	if (tx_channel == -1 && rx_channel == -1) {
        	tx_channel = tx_chans->data[0];
        	rx_channel = rx_chans->data[0];
    	}
    	printf("Using transmit channel %d and receive channel %d.\n", tx_channel, rx_channel);

   
    	rc = single_transfer_test(axidma_dev, nSourceId, nDestId, tx_channel, rx_channel,tx_buf, tx_size, rx_buf, rx_size);
    	if (rc < 0) {
        	goto free_rx_buf;
    	}
    	printf("Single transfer test successfully completed!\n");

free_rx_buf:
    axidma_free(axidma_dev, rx_buf, rx_size);
free_tx_buf:
    axidma_free(axidma_dev, tx_buf, tx_size);
destroy_axidma:
    axidma_destroy(axidma_dev);
ret:
    return rc;
}
