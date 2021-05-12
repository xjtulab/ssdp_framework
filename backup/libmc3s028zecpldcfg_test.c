#include "libmc3s028zecpldcfg.h"

int main()
{
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

