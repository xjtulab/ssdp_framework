#ifndef FPAG_H
#define FPAG_H
#include "base_device.h"

class DeviceFPGA: public DeviceBase{
public:
    DeviceFPGA(string name, SSDP_HandleID id): DeviceBase(name, id){}
    virtual SSDP_Result DEV_Start() {
        cout<<"fpga dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    };
};
#endif