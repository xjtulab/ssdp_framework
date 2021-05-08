//
// Created by ljw on 6/9/20.
//
//#include "base_app.h"

#include"SSDPServer.hpp"
#include<thread>
#include <iostream>
#include "SSDP.h"
#include "SSDP_API.h"
#include "SSDP_LOG.h"
#include "commandprocess.h"
#include <dirent.h>
// #include "lib_test.h"
// extern "C" {
// #include "libmc3s029zesensorinfoget.h"
// #include "libmc3s028zecpldcfg.h"
// }
using std::cout;
using std::endl;
using std::cerr;

#define SSDP_SERVER_IP "192.168.43.109"
#define SSDP_SERVER_PORT 8080
char *IP = new char[16];
int port;

void recv_from_upper_pc(SSDPServer* server);

void init(){
    strcpy(IP, SSDP_SERVER_IP);
    port = SSDP_SERVER_PORT;
    SSDPServer::set_default_directory();
}

void parse_args(int argc, char **argv){
    int ch;
    while((ch = getopt(argc, argv, "a:d:p:")) != -1){
        switch (ch)
        {
            case 'a':{
                memcpy(IP, optarg, strlen(optarg));
                IP[strlen(optarg)] = '\0';
                break;
            }
            case 'd':{
                SSDPServer::set_directory(optarg);
                break;
            }
            case 'p':{
                port = atoi(optarg);
                break;
            }
            case '?':{
                printf("-%c invalid option\n", (char)optopt);
                printf("usage: ");
                printf("./main [-a IP address] [-d file save directory] [-p port]\n");
                exit(1);
                break;
            }
        }
    }
}


// 线程执行的函数参数


int main(int argc, char **argv) {
    //TODO 框架启动流程设计
    /*
        1、读取设备配置文件，创建设备实例
        2、启动和显控界面的连接，准备接收指令
        3、循环接收指令
    */

    // CmdProcess cmdprocesser;
    // // cmdprocesser.ReceiveCommand("asdf dsf");
    // cmdprocesser.ReceiveCommand("SSDP -s ground  -t framework -f start -a app1");
    // return 0;
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
    
    //系统的初始化
    int ssdp_init_result = SSDP_self_Init();
    if (ssdp_init_result != 0){
        cout<<"ssdp init failed"<<endl;
        // return 1;
    }
    SSDP_show_cur_apps();

    //主板接口测试
    // test_sessor();
    // zecpldcfg_test();
    // upgrade_test(argc, argv);
    // 1、读取设备配置文件，创建设备实例
    //TODO 应该改成扫描文件夹底下文件
    DIR *dirp;
    struct dirent *direntp;
    if((dirp = opendir("devices")) == NULL){
        cout<<"device dir error!"<<endl;
        exit(1);
    }
    while((direntp = readdir(dirp)) != NULL){
        string fname = direntp ->d_name;
        int pos;
        if((pos = fname.find(".xml")) != string::npos){
            SSDP_InstantiateDevice(SSDP_OE_HANDLE_ID, fname.substr(0, pos), "devices/"+fname);
        }
    }
    closedir(dirp);
    //创建应用
    int appid = SSDP_InstantiateApp(0,"myapp1","myapp1.xml");

    //测试设备状态回报
    std::string stat = SSDP_DeviceStatus();
    cout<<stat<<endl;
    
    CmdProcess cmdprocesser;
    cmdprocesser.ReceiveCommand("SSDP -s ground  -t framework -f start -a myapp1");
    init();
    parse_args(argc, argv);
    SSDPServer server;
    if(!server.setup(IP, port, true)){
        exit(1);
    }
    // std::thread recv_thread(recv_from_upper_pc, &server);
    // recv_thread.detach();

    server.do_epoll();
    return 1;
    // 2、启动和显控界面的连接，准备接收指令
    // CmdProcess cmdprocesser;
   
    
    // string c;
    // cout<< SSDP_GetHandleName(0,4,c)<<endl;
    // cout<<c<<endl;
    SSDP_Start(0, SSDP_HandleRequest(0, "myapp1"));
    SSDP_Configure(0, SSDP_HandleRequest(0, "myapp1"), "BCH192_64800-DVBS2@9b2dc56", "abc", "dedf", 10);
    cout<<"test end"<<endl;
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
