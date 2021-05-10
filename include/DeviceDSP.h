#ifndef DEVICEDSP_H
#define DEVICEDSP_H
#include "base_device.h"
#include "DspPublisher.h"

class DeviceDSP: public DeviceBase{
private:
    DspPublisher* pub;
public:
    virtual std::string DEV_Status_Qeury();
    virtual SSDP_Result DEV_Check();
    DeviceDSP(string name, SSDP_HandleID id);
    virtual SSDP_Result DEV_Start();
    virtual SSDP_Result DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie);
    ~DeviceDSP();
};

#endif