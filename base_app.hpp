//
// Created by ljw on 6/9/20.
//
#ifndef BASE_APP_HPP
#define BASE_APP_HPP
#include "SSDP_pre_data.h"
#include "SSDP.h"
using std::string;


//应用基类
class AppBase{
protected:
    ssdp_app_functable* ftable;
    string handle_name;
    SSDP_HandleID handle_id;
public:
    //构造函数
    AppBase(string name, SSDP_HandleID id, ssdp_app_functable* ft): handle_name(name),handle_id(id){
        ftable = ft;
    }
    
    //获取ID和name的接口
    SSDP_HandleID APP_GetHandleID(){ return handle_id;}
    string APP_GetHandleName(){ return handle_name;}
    
    //应用待实现接口
    virtual SSDP_Result APP_Start() =0;
    virtual SSDP_Result APP_Stop() =0;
    virtual SSDP_Result APP_Initialize() =0;
    virtual SSDP_Result APP_ReleaseObject() =0;
    virtual SSDP_Result APP_Write(SSDP_Message buffer, SSDP_Buffer_Size buffer_size) =0;
    virtual SSDP_Result APP_Read(SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) =0;
    virtual SSDP_Result APP_Configure(SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie) =0;
    virtual SSDP_Result APP_Query (SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) =0;

    //析构函数
    virtual ~AppBase(){}
};

//应用创建函数
typedef AppBase* (*APP_Instance_ptr)(string name, SSDP_HandleID id, ssdp_app_functable* ft);
//应用销毁函数
typedef void (*APP_Destroy_ptr)(AppBase*);
#endif
