//
// Created by ljw on 6/9/20.
//
//#include "base_app.hpp"
#include <iostream>
#include "SSDP.h"
#include "SSDP_API.h"
//#include "spdlog/spdlog.h"
using std::cout;
using std::endl;
using std::cerr;


int main() {
    //APP1TEST
    //123safasfd
    int ssdp_init_result = SSDP_self_Init();
    if (ssdp_init_result != 0){
        cout<<"ssdp init failed"<<endl;
        return 1;
    }
    SSDP_show_cur_apps();
    int appid = SSDP_InstantiateApp(0,"myapp1","./app1.so");
    int appid1 = SSDP_InstantiateApp(0,"myapp2","./app2.so");
    SSDP_Start(0,appid1);
    SSDP_Stop(0,appid1);
    SSDP_show_cur_apps();
    SSDP_AbortApp(0,appid);
    SSDP_show_cur_apps();
    SSDP_AbortApp(0,appid1);
    SSDP_show_cur_apps();
    return 0;
}
