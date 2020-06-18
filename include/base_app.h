//
// Created by ljw on 6/9/20.
//
#ifndef BASE_APP_HPP
#define BASE_APP_HPP
#include "SSDP_PRE_DATA.h"
#include "SSDP.h"
#include "component.h"
#include <map>
#include <iostream>
using std::string;

enum AppState {APP_INSTANTIATED,APP_STOPPED,APP_RUNNING,APP_TESTING,APP_FATAL};


//应用基类
class AppBase{
protected:
    ssdp_app_functable* ftable;
    string handle_name;
    SSDP_HandleID handle_id;
    std::map<int, struct component> component_list;

public:
    //构造函数
    AppBase(string name, SSDP_HandleID app_id, ssdp_app_functable* ft): handle_name(name),handle_id(app_id){
        ftable = ft;
        std::cout<<name<<" inited"<<std::endl;
        //component_list = new map<int,component>;
    }
    
    //获取ID和name的接口
    SSDP_HandleID APP_GetHandleID(){ return handle_id;}
    const string& APP_GetHandleName(){ return handle_name;}
    //添加组件
    SSDP_Result Add_Component(int id_on_app, string filepath, SSDP_HandleID targetdevice, int id_on_device){
        component_list.insert(std::make_pair(id_on_app, component(filepath,targetdevice, id_on_device)));
    }
    
    //应用待实现接口
    virtual SSDP_Result APP_Start(){};
    virtual SSDP_Result APP_Stop(){};
    virtual SSDP_Result APP_Initialize(){};
    virtual SSDP_Result APP_ReleaseObject(){};
    virtual SSDP_Result APP_Write(int comp_id,SSDP_Message buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Read(int comp_id,SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Configure(int comp_id,SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie) {};
    virtual SSDP_Result APP_Query (int comp_id,SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) {};

    //TODO 析构函数
    virtual ~AppBase(){}                                                           
};

//应用创建函数
typedef AppBase* (*APP_Instance_ptr)(string name, SSDP_HandleID id, ssdp_app_functable* ft);
//应用销毁函数
typedef void (*APP_Destroy_ptr)(AppBase*);
#endif
