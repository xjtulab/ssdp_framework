//
// Created by ljw on 6/9/20.
//
//#include "base_app.h"
#include <iostream>
#include "SSDP.h"
#include "SSDP_API.h"
#include "SSDP_LOG.h"
#include "commandprocess.h"
using std::cout;
using std::endl;
using std::cerr;


int main() {
    //TODO 框架启动流程设计
    /*
        1、启动和FC的连接，返回开始启动消息
        2、装载驱动、HAL等，读取平台配置文件，创建设备实例
        3、运行设备测试
        4、读取平台配置文件，决定需要启动的应用
        7、读取应用配置文件，部署应用
    */
    /*
    CmdProcess cmdprocesser;
    cmdprocesser.ReceiveCommand("asdf dsf");
    cmdprocesser.ReceiveCommand("SSDP -s s -t t -f f -a 1");
    return 0;
    */
    /*
    SSDP_Result res = SSDP_LogInit();
    SSDP_Log(0,SSDP_GetErrorQueue(SSDP_WARNING),"sfasfd",3);
    */
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
    cout<< SSDP_HandleRequest(0,"myapp1")<<endl;
    cout<< SSDP_HandleRequest(0,"sadfsa")<<endl;
    string c;
    cout<< SSDP_GetHandleName(0,0,c)<<endl;
    cout<<c<<endl;
    //int appid1 = SSDP_InstantiateApp(0,"myapp2","./app2.so");
    //SSDP_Start(0,appid);
    //SSDP_Stop(0,appid);
    //SSDP_show_cur_apps();
    //SSDP_AbortApp(0,appid);
    //SSDP_show_cur_apps();
    //SSDP_AbortApp(0,appid1);
    //SSDP_show_cur_apps();
    return 0;
}
