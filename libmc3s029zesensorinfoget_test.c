#include "libmc3s029zesensorinfoget.h"

int main(int argc, char *argv[])
{
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
