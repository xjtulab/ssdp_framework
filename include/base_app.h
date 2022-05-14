//
// Created by ljw on 6/9/20.
//
#ifndef BASE_APP_HPP
#define BASE_APP_HPP
#include "SSDP_PRE_DATA.h"
#include "SSDP.h"
#include "component.h"
#include <unistd.h>
#include <map>
#include <vector>
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
        cout<<"add comp name: "<<id_on_app<<endl;
        component_list.insert(std::make_pair(id_on_app, ptr));
    }
    //添加组件参数名/地址对
    SSDP_Result Add_Component_Parameter(string id_on_app, string name, string address){
        cout<<"parameter added "<<name<<endl;
        component_list[id_on_app]->add_parameter(name, address);
        return SSDP_OK;
    }

    //切换sar中的dsp组件位置
    void Sar_switch(){
        string target_dsp = "";
        vector<SSDP_HandleID> source, target;
        auto iter = component_list.rbegin();
        while(iter != component_list.rend()){
            if(iter->second->target_device == ftable->handleRequest(handle_id, "dsp1")){
                //设备是dsp就修改位置，并重构
                source.emplace_back(iter->second->target_device);
                target.emplace_back(ftable->handleRequest(handle_id, "dsp3"));
                iter->second->target_device = ftable->handleRequest(handle_id, "dsp3");
                target_dsp = "dsp3";
                iter++;
                continue;
            }else if(iter->second->target_device == ftable->handleRequest(handle_id, "dsp2")){
                source.emplace_back(iter->second->target_device);
                target.emplace_back(ftable->handleRequest(handle_id, "dsp4"));
                iter->second->target_device = ftable->handleRequest(handle_id, "dsp4");
                iter++;
                continue;
            }else if(iter->second->target_device == ftable->handleRequest(handle_id, "dsp3")){
                source.emplace_back(iter->second->target_device);
                target.emplace_back(ftable->handleRequest(handle_id, "dsp1"));
                iter->second->target_device = ftable->handleRequest(handle_id, "dsp1");
                target_dsp = "dsp1";
                iter++;
                continue;
            }else if(iter->second->target_device == ftable->handleRequest(handle_id, "dsp4")){
                source.emplace_back(iter->second->target_device);
                target.emplace_back(ftable->handleRequest(handle_id, "dsp2"));
                iter->second->target_device = ftable->handleRequest(handle_id, "dsp2");
                iter++;
                continue;
            }else if(iter->second->target_device == ftable->handleRequest(handle_id, "fpga1")){
                //设备是fpga就发送重构指令
                iter++;
                continue;
            }
        }
        for(auto source_id:source){
            ftable->loadDevice(handle_id, source_id, "0", true);
        }
        for(auto target_id:target){
            ftable->loadDevice(handle_id, target_id, component_list.rbegin()->second->file_path, true);
        }
        if(target_dsp == "dsp1"){
            ftable->write(handle_id, ftable->handleRequest(handle_id, "fpga1"), 0, "switch01_radar",14);
            cout<<"fpga write cmd: switch01_radar"<<endl;
        }else if(target_dsp == "dsp3"){
            ftable->write(handle_id, ftable->handleRequest(handle_id, "fpga1"), 0, "switch23_radar",14);
            cout<<"fpga write cmd: switch23_radar"<<endl;
        }
    }

    void Elec_switch(string target_dsp){
        map<string, string> dspNum2FpgaCmd = {
            {"0", "switch0_elec"},
            {"1", "switch1_elec"},
            {"2", "switch2_elec"},
            {"3", "switch3_elec"}
        };
        string t_dsp = "";
        vector<SSDP_HandleID> source, target;
        auto iter = component_list.rbegin();
        while(iter != component_list.rend()){
            if(iter->second->target_device == ftable->handleRequest(handle_id, "dsp1")){
                //设备是dsp就修改位置，并重构
                source.emplace_back(iter->second->target_device);
                target.emplace_back(ftable->handleRequest(handle_id, "dsp3"));
                iter->second->target_device = ftable->handleRequest(handle_id, "dsp3");
                t_dsp = "dsp3";
                iter++;
                continue;
            }else if(iter->second->target_device == ftable->handleRequest(handle_id, "dsp3")){
                source.emplace_back(iter->second->target_device);
                target.emplace_back(ftable->handleRequest(handle_id, "dsp1"));
                iter->second->target_device = ftable->handleRequest(handle_id, "dsp1");
                t_dsp = "dsp1";
                iter++;
                continue;
            }else if(iter->second->target_device == ftable->handleRequest(handle_id, "fpga1")){
                //设备是fpga就发送重构指令
                // ftable->loadDevice(handle_id, iter->second->target_device, iter->second->file_path, true);
                iter++;
                continue;
            }
        }
        for(auto source_id:source){
            ftable->loadDevice(handle_id, source_id, "0", true);
        }
        for(auto target_id:target){
            ftable->loadDevice(handle_id, target_id, component_list.rbegin()->second->file_path, true);
        }
        // sleep(180);
        if(t_dsp == "dsp1"){
            ftable->write(handle_id, ftable->handleRequest(handle_id, "fpga1"), 0, "switch0_elec",14);
            cout<<"fpga write cmd: switch0_elec"<<endl;
        }else if(t_dsp == "dsp3"){
            ftable->write(handle_id, ftable->handleRequest(handle_id, "fpga1"), 0, "switch2_elec",14);
            cout<<"fpga write cmd: switch2_elec"<<endl;
        }

    }
    
    //应用待实现接口
    virtual SSDP_Result SAR_Start(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.rbegin();
        while(iter != component_list.rend()){
            //TODO 需要传输compi地址参数，修改组件空间的02地址？
            if(!ftable->start(handle_id, iter->second->target_device)){
                res = SSDP_ERROR;
            }
            iter++;
        }
        return res;
    };     
    virtual SSDP_Result DVB_Start(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.begin();
        if(!ftable->start(handle_id, iter->second->target_device)){
            res = SSDP_ERROR;
        }
        return res;
    };
    virtual SSDP_Result ELEC_Start(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.begin();
        while(iter != component_list.end()){
            //TODO 需要传输compi地址参数，修改组件空间的02地址？
            if(!ftable->start(handle_id, iter->second->target_device)){
                res = SSDP_ERROR;
            }
            iter++;
        }
        return res;
    };                                                             
    virtual SSDP_Result SAR_Stop(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.begin();
        while(iter != component_list.end()){
            //TODO 需要传输compi地址参数，修改组件空间的02地址？
            if(!ftable->stop(handle_id, iter->second->target_device)){
                res = SSDP_ERROR;
            }
            iter++;
        }
        return res;
    };
    virtual SSDP_Result DVB_Stop(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.begin();
        if(!ftable->stop(handle_id, iter->second->target_device)){
            res = SSDP_ERROR;
        }
        return res;
    };
    virtual SSDP_Result ELEC_Stop(){
        SSDP_Result res = SSDP_OK;
        auto iter = component_list.rbegin();
        while(iter != component_list.rend()){
            //TODO 需要传输compi地址参数，修改组件空间的02地址？
            if(!ftable->stop(handle_id, iter->second->target_device)){
                res = SSDP_ERROR;
            }
            iter++;
        }
        return res;
    };
    virtual SSDP_Result APP_Initialize(){};
    virtual SSDP_Result APP_ReleaseObject(){};
    virtual SSDP_Result APP_Write(int comp_id,SSDP_Message buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Read(int comp_id,SSDP_Message& buffer, SSDP_Buffer_Size buffer_size) {};
    virtual SSDP_Result APP_Configure(string comp_id,SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size) {
        //需要输入设备id，组件在设备上的id？地址和参数
        //nanm就是地址，参数值还是参数值
        cout<<"app config"<<endl;
        SSDP_Result res = ftable->config(handle_id, component_list[comp_id]->target_device, component_list[comp_id]->comp_id_on_device, 
            component_list[comp_id]->get_address(name), value, value_size);
        return res;
    };
    virtual SSDP_Result APP_Query (int comp_id,SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size) {};
    //TODO 析构函数
    virtual ~AppBase(){
        cout<<"deleteing app "<<this->handle_name<<endl;
    }                                                           
};

//应用创建函数
typedef AppBase* (*APP_Instance_ptr)(string name, SSDP_HandleID id, ssdp_app_functable* ft);
//应用销毁函数
typedef void (*APP_Destroy_ptr)(AppBase*);
#endif
