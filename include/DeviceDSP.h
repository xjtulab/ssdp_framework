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
    ~DeviceDSP();
};

#endif