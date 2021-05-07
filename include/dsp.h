#ifndef DSP_H
#define DSP_H
#include "base_device.h"

class DeviceDSP: public DeviceBase{
public:
    DeviceDSP(string name, SSDP_HandleID id): DeviceBase(name, id){}
    virtual SSDP_Result DEV_Start() {
        cout<<"dsp dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    };
};
#endif