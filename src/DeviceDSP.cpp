#include "DeviceDSP.h"
#include <unistd.h>
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
    #ifdef ARM_BUILD
        // uxr_delete_session(&session);
        // uxr_close_udp_transport(&transport);
        delete pub;
    #endif
}

SSDP_Result DeviceDSP::DEV_Start(){
    // Write topics
    // bool connected = true;
    // uint32_t count = 0;
    // HelloWorld topic = {++count, "SSDP FRAME!"};
    // ucdrBuffer ub;
    // uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
    // uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
    // HelloWorld_serialize_topic(&ub, &topic);

    // printf("Send topic: %s, id: %i\n", topic.message, topic.index);
    // connected = uxr_run_session_time(&session, 1000); 
    // sleep(10);
    // bool connected = true;
    // uint32_t count = 0;
    // while(connected && count < max_topics)
    // {
    //     cout<<"start seding"<<endl;
    //     // char mesg[255];
        // string tmp = "SSDP FRAME!"+to_string(count);
        // strcpy(mesg, tmp.c_str());
        // // char *mesg = (char*)("SSDP FRAME!"+to_string(count)).c_str();
        // HelloWorld topic;
        // topic.index = ++count;
        // strcpy(topic.message, tmp.c_str());
        // //  = {++count, *mesg};

    //     ucdrBuffer ub;
    //     uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
    //     uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
    //     HelloWorld_serialize_topic(&ub, &topic);

    //     printf("Send topic: %s, id: %i\n", topic.message, topic.index);
    //     connected = uxr_run_session_time(&session, 1000);
    // }
    cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    #ifdef ARM_BUILD
        bool res = pub->send_info("start",true);
        // sleep(1);
        res = pub->send_info("config dsp1 CodeRate 0x10000", true);
        // sleep(1);
        res = pub->send_info("stop", true);
        // sleep(1);
        res = pub->send_info("query", true);
        // sleep(1);
        res = pub->send_info("init", true);
        return res? SSDP_OK:SSDP_ERROR;
    #endif
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
    // ip = (char*)ip1.c_str();
    // port = (char*)port1.c_str();
    // if(!uxr_init_udp_transport(&transport, &udp_platform, UXR_IPv4, ip, port))
    // {
    //     printf("Error at create transport.\n");
    //     return ;
    // }
    // uxr_init_session(&session, &transport.comm, 0xAAAABBBB);
    // if(!uxr_create_session(&session))
    // {
    //     printf("Error at create session.\n");
    //     return ;
    // }
    // reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    // uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    // // Create entities
    // participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    // participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);

    // topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    // string topic_xml = "<dds>"
    //                         "<topic>"
    //                             "<name>"+topic_name1+"</name>"
    //                             "<dataType>HelloWorld</dataType>"
    //                         "</topic>"
    //                     "</dds>";
    // topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml.c_str(), UXR_REPLACE);

    // publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    // const char* publisher_xml = "";
    // publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id, publisher_xml, UXR_REPLACE);

    // datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    // string datawriter_xml = "<dds>"
    //                             "<data_writer>"
    //                                 "<topic>"
    //                                     "<kind>NO_KEY</kind>"
    //                                     "<name>"+topic_name1+"</name>"
    //                                     "<dataType>HelloWorld</dataType>"
    //                                 "</topic>"
    //                             "</data_writer>"
    //                         "</dds>";
    // datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id, datawriter_xml.c_str(), UXR_REPLACE);

    // // Send create entities message and wait its status
    // requests[0] = participant_req;
    // requests[1] = topic_req;
    // requests[2] = publisher_req;
    // requests[3] = datawriter_req;
    // // uint16_t requests[4] = {participant_req, topic_req, publisher_req, datawriter_req};
    // if(!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
    // {
    //     printf("Error at create entities: participant: %i topic: %i publisher: %i darawriter: %i\n", status[0], status[1], status[2], status[3]);
    //     return ;
    // }

    // bool connected = true;
    // uint32_t count = 0;
    // HelloWorld topic = {++count, "EMPTY TEST!"};
    // ucdrBuffer ub;
    // uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
    // uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
    // HelloWorld_serialize_topic(&ub, &topic);

    // printf("Send topic: %s, id: %i\n", topic.message, topic.index);
    // connected = uxr_run_session_time(&session, 1000); 
    // Write topics
    // bool connected = true;
    // uint32_t count = 0;
    // while(connected && count < max_topics)
    // {
    //     HelloWorld topic = {++count, "SSDP FRAME!"};

    //     ucdrBuffer ub;
    //     uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
    //     uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
    //     HelloWorld_serialize_topic(&ub, &topic);

    //     printf("Send topic: %s, id: %i\n", topic.message, topic.index);
    //     connected = uxr_run_session_time(&session, 1000);
    // }

    // Delete resources
    // uxr_delete_session(&session);
    // uxr_close_udp_transport(&transport);

    // return ;

    char* ip_tmp = (char*)ip.c_str();
    char* port_tmp = (char*)port.c_str();
    uint32_t nValude = 0;
    sscanf(session_key.c_str(), "%x", &nValude);
    pub = new DspPublisherTwo(ip_tmp, port_tmp, topic_name, nValude, nValude+1);
    pub->send_info("EMPTY TEST",false);
}