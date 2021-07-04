#include "devices/DeviceDSP.h"
#include <unistd.h>
// #define ARM_BUILD
#ifdef ARM_BUILD
    extern "C"{
        #include "58/libmc3s028zecpldcfg.h"
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
    #ifdef ARM_BUILD
        delete pub;
    #endif
}

SSDP_Result DeviceDSP::DEV_Start(){
    cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    #ifdef ARM_BUILD
        bool res = pub->send_info("start",true);
        return res? SSDP_OK:SSDP_ERROR;
    #else
        return SSDP_OK;
    #endif
}

SSDP_Result DeviceDSP::DEV_Stop(){
    cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is stoping"<<endl;
    #ifdef ARM_BUILD
        bool res = pub->send_info("stop",true);
        return res? SSDP_OK:SSDP_ERROR;
    #else
        return SSDP_OK;
    #endif
}

SSDP_Result DeviceDSP::DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie){
    cout<<"on dev "<<handle_name<<" comp: "<<comp_id<<" property: "<<name<<" is changing to: "<<value<<endl;
    std::string cmd_str = "config "+comp_id+" "+name+" "+value;
    char* cmd = (char*)cmd_str.c_str();
    #ifdef ARM_BUILD   
        bool res = pub->send_info(cmd, true);
        return res? SSDP_OK:SSDP_ERROR;
    #else  
        return SSDP_OK;
    #endif

}



std::string DeviceDSP::DEV_Status_Qeury(){
    float signal_processing_board_voltage1 = 0;
    float signal_processing_board_temperature1 = 0;
    bool get_signal_processing_board_dsp_upgrade_complete_status = false;
    int  get_signal_processing_board_dsp_upgrade_complete_time = 0;
    char num = handle_name[handle_name.size()-1];
    #ifdef ARM_BUILD
        unsigned int* map_bram_ctrl_address;
        map_bram_ctrl_address = spdcpldop_init();
        switch (num)
        {
            case '1':
                /* code */
                get_signal_processing_board_dsp_upgrade_complete_status = get_signal_processing_board_dsp1_upgrade_complete_status(map_bram_ctrl_address);
                get_signal_processing_board_dsp_upgrade_complete_time = get_signal_processing_board_dsp1_upgrade_complete_time(map_bram_ctrl_address);
                break;
            case '2':
                get_signal_processing_board_dsp_upgrade_complete_status = get_signal_processing_board_dsp2_upgrade_complete_status(map_bram_ctrl_address);
                get_signal_processing_board_dsp_upgrade_complete_time = get_signal_processing_board_dsp2_upgrade_complete_time(map_bram_ctrl_address);
                break;
            case '3':
                get_signal_processing_board_dsp_upgrade_complete_status = get_signal_processing_board_dsp3_upgrade_complete_status(map_bram_ctrl_address);
                get_signal_processing_board_dsp_upgrade_complete_time = get_signal_processing_board_dsp3_upgrade_complete_time(map_bram_ctrl_address); 
                break;
            case '4':
                get_signal_processing_board_dsp_upgrade_complete_status = get_signal_processing_board_dsp4_upgrade_complete_status(map_bram_ctrl_address);
                get_signal_processing_board_dsp_upgrade_complete_time = get_signal_processing_board_dsp4_upgrade_complete_time(map_bram_ctrl_address);
                break;
            default:
                break;
        }
        signal_processing_board_voltage1 = get_signal_processing_board_voltage1(map_bram_ctrl_address);
        signal_processing_board_temperature1 = get_signal_processing_board_temperature1(map_bram_ctrl_address);
        spdcpldop_release();
    #endif
    std::string res = "<device>"
		                  "<Id>" + handle_name + "</Id>"
		                  "<vol>"+std::to_string(signal_processing_board_voltage1)+"</vol>"
                          "<temp>"+std::to_string(signal_processing_board_temperature1)+"</temp>"
                          "<sta>"+std::to_string(get_signal_processing_board_dsp_upgrade_complete_status)+"</sta>"
                          "<time>"+std::to_string(get_signal_processing_board_dsp_upgrade_complete_time)+"</time>"
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
    pub = new DspPublisherTwo(ip_tmp, port_tmp, topic_name, nValude, nValude+1);
    // pub->send_info("EMPTY TEST",false);
}

SSDP_Result  DeviceDSP::DEV_Load(string filename, bool ifNewCode){
    cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is loading"<<endl;
    // string cmd = "reconstruct "+filename;
    std::string cmd_str = "reconstruct "+filename;
    char* cmd = (char*)cmd_str.c_str();
    #ifdef ARM_BUILD
        bool res = false;
        if(ifNewCode){
            res = pub->send_info(cmd,false);
        }else{
            pub->establish_connection();
        }
        // pub->send_info("EMPTY TEST",false);
        return res? SSDP_OK:SSDP_ERROR;
    #else
        return SSDP_OK;
    #endif 
}