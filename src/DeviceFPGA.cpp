#include "DeviceFPGA.h"
#ifdef ARM_BUILD
extern "C"{
    #include "libmc3s028zecpldcfg.h"
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