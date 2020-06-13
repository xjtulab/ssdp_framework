//
// Created by ljw on 6/9/20.
//
//#include "base_app.h"
#include <iostream>
#include "SSDP.h"
#include "SSDP_API.h"
#include "SSDP_LOG.h"
using std::cout;
using std::endl;
using std::cerr;


int main() {
    SSDP_Result res = SSDP_LogInit();
    SSDP_Log(0,SSDP_GetErrorQueue(SSDP_WARNING),"sfasfd",3);
    /*
    auto file_logger = spdlog::rotating_logger_mt("file_logger", "tmp/myfilename", 1024 * 1024 * 5, 3);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("tmp/logfile", 1024*1024, 5, false);
    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
    spdlog::logger logger("multi_sink", sink_list.begin(), sink_list.end());
    int i =0;
    while (i< 10000){
        logger.warn("Hel11lo, {}!", "World");
        i++;
    }*/
    //APP1TEST
    
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
