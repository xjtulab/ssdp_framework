#ifndef DEVICEDSP_H
#define DEVICEDSP_H
#include "base_device.h"


class DeviceDSP: public DeviceBase{
public:
    virtual std::string DEV_Status_Qeury();
    virtual SSDP_Result DEV_Check();
    DeviceDSP(string name, SSDP_HandleID id);
    virtual SSDP_Result DEV_Start() {
        cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    }
};

#endif