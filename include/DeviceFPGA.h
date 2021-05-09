#ifndef DEVICEFPAG_H
#define DEVICEFPAG_H
#include "base_device.h"


class DeviceFPGA: public DeviceBase{
public:
    virtual std::string DEV_Status_Qeury();
    virtual SSDP_Result DEV_Check();
    DeviceFPGA(string name, SSDP_HandleID id);
    virtual SSDP_Result DEV_Start() {
        cout<<"fpga dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    }
    
};




#endif