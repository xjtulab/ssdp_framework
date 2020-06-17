#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "SSDP_PRE_DATA.h"
#include "SSDP.h"
using std::string;

enum resource_type {BYTE, GATE};
struct device_resource{
    int total;
    int remain;
    resource_type type;
    device_resource(int to, resource_type ty): total(to),remain(to),type(ty) {}
};
//设备基类
class DeviceBase{
protected:
    SSDP_HandleID handle_id;
    string handle_name;
    device_resource* resource;

    
public:
    //构造函数
    DeviceBase(string name, SSDP_HandleID id): handle_name(name), handle_id(id) {
        //TODO 资源的初始化？通过配置文件还是可以直接从设备读取
        resource = new device_resource(100,GATE);
    }
    //设备待实现接口
    virtual SSDP_Result APP_Start() =0;
    virtual SSDP_Result APP_Stop() =0;
    virtual SSDP_Result APP_Initialize() =0;
    virtual SSDP_Result APP_ReleaseObject() =0;
    virtual SSDP_Result APP_Write(SSDP_Message buffer, SSDP_Buffer_Size buffer_size) =0;
    virtual SSDP_Result APP_Read(SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) =0;
    virtual SSDP_Result APP_Configure(SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie) =0;
    virtual SSDP_Result APP_Query (SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) =0;
    //设备独有功能
    virtual SSDP_Result DEV_Open() =0;
    virtual SSDP_Result DEV_Close() =0;
    virtual SSDP_Result DEV_Load(string filename) =0;
    virtual SSDP_Result DEV_Flush() =0;
    virtual SSDP_Result DEV_Reset() =0;
    virtual SSDP_Result DEV_Unload() =0;
    //TODO 设置中断的功能要怎么实现？
    virtual SSDP_Result DEV_SetISR() =0;

    //TODO 析构函数
    ~DeviceBase(){}
};

typedef DeviceBase* (*APP_Instance_ptr)(string name, SSDP_HandleID id);
//应用销毁函数
typedef void (*APP_Destroy_ptr)(DeviceBase*);

#endif