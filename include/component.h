#ifndef COMPONENT_H
#define COMPONENT_H
#include "SSDP_PRE_DATA.h"
using std::string;

struct component
{
    string file_path;
    SSDP_HandleID target_device;
    component(string &filepath, SSDP_HandleID targetdevice){
        file_path = filepath;
        target_device = targetdevice;
    }
};


#endif