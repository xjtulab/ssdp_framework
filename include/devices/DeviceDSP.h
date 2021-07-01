#ifndef DEVICEDSP_H
#define DEVICEDSP_H
#include "base_device.h"
#include "microdds/HelloWorld.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h> //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi
class DeviceDSP: public DeviceBase{
public:
    virtual std::string DEV_Status_Qeury();
    virtual SSDP_Result DEV_Check();
    DeviceDSP(string name, SSDP_HandleID id);
    virtual SSDP_Result DEV_Start();
    virtual SSDP_Result DEV_Stop();
    virtual SSDP_Result DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie);
    virtual void DEV_SetPub(string ip, string port, string topic_name, string session_key);
    virtual SSDP_Result DEV_Load(string filename, bool ifNewCode);
    ~DeviceDSP();
};

#endif