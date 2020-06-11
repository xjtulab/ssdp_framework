//
// Created by ljw on 6/9/20.
//
#ifndef BASE_APP_HPP
#define BASE_APP_HPP
#include "SSDP.h"
using std::string;


//应用基类
class AppBase{
protected:
    struct ssdp_app_functable* ftable;
    string handle_name;
    int handle_id;
public:
    //构造函数
    AppBase(string name, int id, struct ssdp_app_functable* ft): handle_name(name),handle_id(id){
        ftable = ft;
    }
    
    //获取ID和name的接口
    int APP_GetHandleID(){ return handle_id;}
    string APP_GetHandleName(){ return handle_name;}
    
    //应用待实现接口
    virtual int APP_Start() =0;
    virtual int APP_Stop() =0;
    virtual int APP_Initialize() =0;
    virtual int APP_ReleaseObject() =0;
    virtual int APP_Write(string buffer, int buffer_size) =0;
    virtual int APP_Read(string* buffer, int buffer_size) =0;
    virtual int APP_Configure(string name, string value, int value_szie) =0;
    virtual int APP_Query (string name, string* value, int value_size) =0;

    //析构函数
    virtual ~AppBase(){}
};

//应用创建函数
typedef AppBase* (*APP_Instance_ptr)(string name, int id, struct ssdp_app_functable* ft);
//应用销毁函数
typedef void (*APP_Destroy_ptr)(AppBase*);
#endif
