#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "SSDP_PRE_DATA.h"
#include "SSDP.h"
#include <iostream>
#include "microdds/DspPublisherTwo.h"
using std::string;
using namespace std;


//TODO 设备状态怎么确定？有哪些状态？
enum DeviceState {DEVICE_READY,DEVICE_RUNNIG,DEVICE_CLOSED};
struct device_resource{
    int total;
    int remain;
    ResourceType type;
    device_resource(int to, ResourceType ty): total(to),remain(to),type(ty) {}
};
//设备基类
class DeviceBase{
protected:
    SSDP_HandleID handle_id;
    string handle_name;
    device_resource* resource;
    DeviceState state;
    DspPublisherTwo* pub;
    //TODO 设备是否有自己的属性?
public:
    //构造函数
    DeviceBase(string name, SSDP_HandleID id): handle_name(name), handle_id(id) {
        //TODO 资源的初始化？通过配置文件还是可以直接从设备读取
        resource = new device_resource(100,GATE);
        state = DEVICE_READY;
    }

    //获取ID和name的接口
    SSDP_HandleID DEV_GetHandleID(){ return handle_id;}
    const string& DEV_GetHandleName(){ return handle_name;}

    virtual SSDP_Result DEV_Start(string appName) {
        cout<<"dev "<<this->DEV_GetHandleName()<<" is starting"<<endl;
    };
    virtual SSDP_Result DEV_Stop(string appName) {
        cout<<"dev "<<this->DEV_GetHandleName()<<" is stopping"<<endl;
    };
    //添加向设备发送指令的功能
    virtual SSDP_Result DEV_Write(string msg){};
    virtual SSDP_Result APP_Initialize() {};
    virtual SSDP_Result APP_ReleaseObject() {};
    virtual SSDP_Result APP_Write(SSDP_Message buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Read(SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie) {
        cout<<"on dev "<<handle_name<<" comp: "<<comp_id<<" property: "<<name<<" is changing to: "<<value<<endl;
    };
    virtual SSDP_Result APP_Query (SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) {};
    //设备独有功能
    virtual void DEV_SetPub(string ip, string port, string topic_name, string session_key) {};
    virtual std::string DEV_Status_Qeury() = 0;
    virtual SSDP_Result DEV_Check() = 0;
    virtual SSDP_Result DEV_Open() {};
    virtual SSDP_Result DEV_Close() {};
    virtual SSDP_Result DEV_Load(string filename, bool ifNewCode) {
        cout<<"device "<<this->DEV_GetHandleName()<<" is loading"<<endl;
    }; 
    virtual SSDP_Result DEV_Flush() {};
    virtual SSDP_Result DEV_Reset() {};
    virtual SSDP_Result DEV_Unload() {};
    //TODO 设置中断的功能要怎么实现？
    virtual SSDP_Result DEV_SetISR() {};


    // virtual SSDP_Result APP_Start() =0;
    // virtual SSDP_Result APP_Stop() =0;
    // virtual SSDP_Result APP_Initialize() =0;
    // virtual SSDP_Result APP_ReleaseObject() =0;
    // virtual SSDP_Result APP_Write(SSDP_Message buffer, SSDP_Buffer_Size buffer_size) =0;
    // virtual SSDP_Result APP_Read(SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) =0;
    // virtual SSDP_Result APP_Configure(SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie) =0;
    // virtual SSDP_Result APP_Query (SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) =0;
    // //设备独有功能
    // virtual SSDP_Result DEV_Open() =0;
    // virtual SSDP_Result DEV_Close() =0;
    // virtual SSDP_Result DEV_Load(string filename) =0;
    // virtual SSDP_Result DEV_Flush() =0;
    // virtual SSDP_Result DEV_Reset() =0;
    // virtual SSDP_Result DEV_Unload() =0;
    // //TODO 设置中断的功能要怎么实现？
    // virtual SSDP_Result DEV_SetISR() =0;

    //TODO 析构函数
    virtual ~DeviceBase(){}
};

typedef DeviceBase* (*DEV_Instance_ptr)(string name, SSDP_HandleID id);
//应用销毁函数
typedef void (*DEV_Destroy_ptr)(DeviceBase*);

#endif