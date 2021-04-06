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
using namespace std;

enum AppState {APP_INSTANTIATED,APP_STOPPED,APP_RUNNING,APP_TESTING,APP_FATAL};

//TODO 使用协议类或者句柄类来实现APP的定义，分离接口定义和实现，简化编译依赖
//但是会多消耗内存和运行时间

//应用基类
class AppBase{
protected:
    ssdp_app_functable* ftable;
    string handle_name;
    SSDP_HandleID handle_id;
    std::map<string, struct component*> component_list;
    AppState state;

public:
    //构造函数
    AppBase(string name, SSDP_HandleID app_id, ssdp_app_functable* ft): handle_name(name),handle_id(app_id),ftable(ft){
        state = APP_INSTANTIATED;
        std::cout<<name<<" inited"<<std::endl;
    }
    
    //获取ID和name的接口
    SSDP_HandleID APP_GetHandleID(){ return handle_id;}
    const string& APP_GetHandleName(){ return handle_name;}
    //添加组件
    SSDP_Result Add_Component(string id_on_app, string filepath, SSDP_HandleID targetdevice, string id_on_device){
        component* ptr = new component(filepath,targetdevice, id_on_device);
        component_list.insert(std::make_pair(id_on_app, ptr));
    }
    
    //应用待实现接口
    virtual SSDP_Result APP_Start(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.begin();
        while(iter != component_list.end()){
            if(!ftable->start(handle_id, iter->second->target_device)){
                res = SSDP_ERROR;
            }
            iter++;
        }
        return res;
    };                                                                         
    virtual SSDP_Result APP_Stop(){};
    virtual SSDP_Result APP_Initialize(){};
    virtual SSDP_Result APP_ReleaseObject(){};
    virtual SSDP_Result APP_Write(int comp_id,SSDP_Message buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Read(int comp_id,SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Configure(string comp_id,SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size) {
        SSDP_Result res = ftable->config(handle_id, component_list[comp_id]->target_device, component_list[comp_id]->comp_id_on_device, name, value, value_size);
        return res;
    };
    virtual SSDP_Result APP_Query (int comp_id,SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) {};

    //TODO 析构函数
    virtual ~AppBase(){}                                                           
};

//应用创建函数
typedef AppBase* (*APP_Instance_ptr)(string name, SSDP_HandleID id, ssdp_app_functable* ft);
//应用销毁函数
typedef void (*APP_Destroy_ptr)(AppBase*);
#endif
