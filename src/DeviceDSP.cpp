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

DeviceDSP::~DeviceDSP(){
    cout<<"deleteing dsp device"<<endl;
    delete pub;
}

SSDP_Result DeviceDSP::DEV_Start(){
    cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    #
    bool res = pub->send_cmd("start");
    res = pub->send_cmd("config dsp1 CodeRate 0x10000");
    res = pub->send_cmd("stop");
    res = pub->send_cmd("query");
    res = pub->send_cmd("init");
    return res? SSDP_OK:SSDP_ERROR;
    return SSDP_OK;
}

SSDP_Result DeviceDSP::DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie){
    cout<<"on dev "<<handle_name<<" comp: "<<comp_id<<" property: "<<name<<" is changing to: "<<value<<endl;
    std::string cmd = "config "+comp_id+" "+name+" "+value;
    // bool res = pub->send_cmd(cmd.c_str());
    // res = pub->send_cmd("stop");
    // res = pub->send_cmd("query");
    // res = pub->send_cmd("init");
    cout<<"on dev "<<handle_name<<" comp: "<<comp_id<<" property: "<<name<<" is changing to: "<<value<<endl;
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

void DeviceDSP::DEV_SetPub(string ip, string port, string topic_name, string session_key){
    cout<<ip<<" "<<" "<<port<<" "<<" "<<topic_name<<" "<<session_key<<endl; 
    char* ip_tmp = (char*)ip.c_str();
    char* port_tmp = (char*)port.c_str();
    uint32_t nValude = 0;
    sscanf(session_key.c_str(), "%x", &nValude);
    pub = new DspPublisher(ip_tmp, port_tmp, topic_name, nValude);
}