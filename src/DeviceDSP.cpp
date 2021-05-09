#include "DeviceDSP.h"
// #define ARM_BUILD
#ifdef ARM_BUILD
extern "C"{
    #include "libmc3s028zecpldcfg.h"
}
#endif

//重载函数实现

DeviceDSP::DeviceDSP(string name, SSDP_HandleID id): DeviceBase(name, id){
    SSDP_Result DEV_OK =  DEV_Check();
    if(DEV_OK == SSDP_OK){
        cout<<"Device Check ok"<<endl;
        state = DEVICE_READY;
    }else{
        cout<<"Warning: device not ready"<<endl;
    }
}

std::string DeviceDSP::DEV_Status_Qeury(){
    float signal_processing_board_voltage1 = 0;
    float signal_processing_board_temperature1 = 0;
#ifdef ARM_BUILD
    unsigned int* map_bram_ctrl_address;
    map_bram_ctrl_address = spdcpldop_init();
    signal_processing_board_voltage1 = get_signal_processing_board_voltage1(map_bram_ctrl_address);
    signal_processing_board_temperature1 = get_signal_processing_board_temperature1(map_bram_ctrl_address);
    spdcpldop_release();
#endif
    std::string res = "<device>"
		                  "<Id>dsp</Id>"
		                  "<vol>"+std::to_string(signal_processing_board_voltage1)+"</vol>"
                          "<temp>"+std::to_string(signal_processing_board_temperature1)+"</temp>"
                       "</device>";
    return res;
}

SSDP_Result DeviceDSP::DEV_Check(){
    SSDP_Result res = SSDP_OK;
#ifdef ARM_BUILD
    unsigned int* map_bram_ctrl_address;
    map_bram_ctrl_address = spdcpldop_init();
    if(get_signal_processing_board_dsp1_srio_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp1_srio_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp1_srio_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp2_srio_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp2_srio_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp2_srio_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp3_srio_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp3_srio_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp3_srio_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp4_srio_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp4_srio_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp4_srio_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp1_ge_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp1_ge_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp1_ge_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp2_ge_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp2_ge_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp2_ge_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp3_ge_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp3_ge_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp3_ge_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp4_ge_link_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp4_ge_link_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp4_ge_link_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp1_core_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp1_core_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp1_core_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp2_core_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp2_core_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp2_core_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp3_core_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp3_core_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp3_core_status is not ok"<<endl;
    }
    if(get_signal_processing_board_dsp4_core_status(map_bram_ctrl_address)){
        cout<<"signal_processing_board_dsp4_core_status is ok"<<endl;
    }else{
        res = SSDP_ERROR;
        cout<<"signal_processing_board_dsp4_core_status is not ok"<<endl;
    }
    spdcpldop_release();
#endif
    return res;
}