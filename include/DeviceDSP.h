#ifndef DEVICEDSP_H
#define DEVICEDSP_H
#include "base_device.h"
#include "HelloWorld.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h> //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#define STREAM_HISTORY  8
#define BUFFER_SIZE     UXR_CONFIG_UDP_TRANSPORT_MTU * STREAM_HISTORY
class DeviceDSP: public DeviceBase{
private:
    char* ip;
    //  = "10.119.84.193";
    char* port;
    //  = "2018";
    uint32_t max_topics = 2018;
    uxrUDPTransport transport;
    uxrUDPPlatform udp_platform;
    uxrSession session;
    uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
    uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
    uxrStreamId reliable_out;
    uxrObjectId participant_id;
    const char* participant_xml = "<dds>"
                                      "<participant>"
                                          "<rtps>"
                                              "<name>default_xrce_participant</name>"
                                          "</rtps>"
                                      "</participant>"
                                  "</dds>";
    uint16_t participant_req;
    uxrObjectId topic_id;
    // char* topic_xml = "<dds>"
    //                         "<topic>"
    //                             "<name>%s</name>"
    //                             "<dataType>HelloWorld</dataType>"
    //                         "</topic>"
    //                     "</dds>";
    uint16_t topic_req;
    uxrObjectId publisher_id;
    uint16_t publisher_req;
    uxrObjectId datawriter_id;
    // char* datawriter_xml = "<dds>"
    //                             "<data_writer>"
    //                                 "<topic>"
    //                                     "<kind>NO_KEY</kind>"
    //                                     "<name>%s</name>"
    //                                     "<dataType>HelloWorld</dataType>"
    //                                 "</topic>"
    //                             "</data_writer>"
    //                         "</dds>";
    uint16_t datawriter_req;
    uint8_t status[4];
    uint16_t requests[4];
public:
    virtual std::string DEV_Status_Qeury();
    virtual SSDP_Result DEV_Check();
    DeviceDSP(string name, SSDP_HandleID id);
    virtual SSDP_Result DEV_Start();
    virtual SSDP_Result DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie);
    virtual void DEV_SetPub(string ip, string port, string topic_name, string session_key);
    ~DeviceDSP();
};

#endif