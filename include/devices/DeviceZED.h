#ifndef DEVICEZED_H
#define DEVICEZED_H
#include "base_device.h"
#include "zed_client.h"

class DeviceZED: public DeviceBase{
private:
    int sockfd;
    struct sockaddr_in serv_addr;
public:
    virtual std::string DEV_Status_Qeury();
    virtual SSDP_Result DEV_Check();
    DeviceZED(string name, SSDP_HandleID id);
    virtual SSDP_Result DEV_Start();
    // virtual SSDP_Result DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie);
    virtual void DEV_SetPub(string ip, string port, string topic_name, string session_key);
    ~DeviceZED();
};

#endif