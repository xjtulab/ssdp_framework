#ifndef COMPONENT_H
#define COMPONENT_H
#include "SSDP_PRE_DATA.h"
#include <map>
#include <iostream>
using std::string;
using std::map;

struct component
{
    string file_path;
    SSDP_HandleID target_device;
    map<string, string> parameters;
    //TODO 需不需要添加资源占用信息
    //TODO 设备上ID暂定为int
    string comp_id_on_device;
    component(string &filepath, SSDP_HandleID targetdevice, string id){
        file_path = filepath;
        target_device = targetdevice;
        comp_id_on_device = id;
    }
    void add_parameter(string& name, string& address){
        parameters[name] = address;
    }
    string get_address(string& name){
        return parameters[name];
    }
};


#endif