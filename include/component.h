#ifndef COMPONENT_H
#define COMPONENT_H
#include "SSDP_PRE_DATA.h"
using std::string;

struct component
{
    string file_path;
    SSDP_HandleID target_device;
    //TODO 需不需要添加资源占用信息
    //TODO 设备上ID暂定为int
    int comp_id_on_device;
    component(string &filepath, SSDP_HandleID targetdevice, int id){
        file_path = filepath;
        target_device = targetdevice;
        comp_id_on_device = id;
    }
};


#endif