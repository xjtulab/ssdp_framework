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
#include "microdds/DspPublisherTwo.h"

// #include "DspPublisher.h"
// #include "lib_test.h"
// extern "C" {
// #include "libmc3s029zesensorinfoget.h"
// #include "libmc3s028zecpldcfg.h"
// }
using std::cout;
using std::endl;
using std::cerr;

#define SSDP_SERVER_IP "10.119.84.190"
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


int main(int argc, char **argv) {

    //TODO 框架启动流程设计
    /*
        1、读取设备配置文件，创建设备实例
        2、启动和显控界面的连接，准备接收指令
        3、循环接收指令
    */
    
    //系统的初始化
    int ssdp_init_result = SSDP_self_Init();
    if (ssdp_init_result != 0){
        cout<<"ssdp init failed"<<endl;
        // return 1;
    }
    //启动agent
    // system("sh /SSDP/dds/kill.sh");
    // system("/SSDP/dds/agent udp4 -p 2019 > /SSDP/dds/log.txt &");
    // sleep(1);
    SSDP_show_cur_apps();
    // 1、读取设备配置文件，创建设备实例
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
    // cout<<SSDP_GetDeviceList()<<endl;
    // int appid = SSDP_InstantiateApp(0,"myapp1","myapp1.xml");
    int appid = SSDP_InstantiateApp(0,"elec_reco","elec_reco.xml");

    //测试设备状态回报
    // std::string stat = SSDP_DeviceStatus();
    // cout<<stat<<endl;
    
    //测试FPGA UPGRADE
    // SSDP_HandleID device_id = SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "fpga1");
    // cout<<"device load "<<SSDP_LoadDevie(SSDP_OE_HANDLE_ID, device_id, "file_path")<<endl;

    //准备指令解析器并测试
    // CmdProcess cmdprocesser;
    // cout<<"---------------------------------------"<<endl;
    // cout<<"--------------------testing tongxin v1---------------"<<endl;
    // sleep(10);
    // while(true){
    //     cout<<"please enter command:"<<endl;
    //     string cmd4 = "-f start -a dianzhen_app1";  //110
    //     string cmd1 = "-f config -a dianzhen_app1 -c processing@6edd4fe2 -n If_thres -v 0d01"; // 5 
    //     string cmd2 = "-f config -a dianzhen_app1 -c processing@6edd4fe2 -n thres -v 0d00000256"; //30
    //     string cmd3 = "-f reconstruct -a dsp1 -v 1"; // 50
    //     string cmd5 = "-f stop -a dianzhen_app1"; // 130
    //     string cmd6 = "-f reconstruct -a fpga1 -v 文件名"; //150
    //     string cmd;
    //     int tmp;
    //     cin >>tmp;
    //     if(tmp > 0 && tmp <10){
    //         cmd = cmd1; //config
    //     }else if(tmp >20 && tmp <40){
    //         cmd = cmd2; // 
    //     }else if(tmp >40 && tmp <60){
    //         cmd = cmd3;
    //     }else if(tmp >60 && tmp <80){
    //         break;
    //     }else if(tmp <100 && tmp >80){
    //         int appid = SSDP_InstantiateApp(0,"dianzhen_app1","dianzhen_app1.xml"); //本来属于load app
    //     }else if(tmp >100 && tmp <120){
    //         cmd = cmd4;
    //     }else if (tmp >120 && tmp <140){
    //         cmd = cmd5;
    //     }else if(tmp > 140 && tmp < 160){
    //         cmd = cmd6;
    //     }
    //     cout<<"cmd is:"<<" SSDP -s ground -t framework "+cmd<<endl;
    //     cmdprocesser.ReceiveCommand("SSDP -s ground -t framework "+cmd);
    //     // sleep(1);
    // }
    // int aaa;
    // cin>>aaa;
    // cmdprocesser.ReceiveCommand("SSDP -s ground -t framework -f stop -a dianzhen_app1");
    // sleep(1);
    // cmdprocesser.ReceiveCommand("SSDP -s ground -t framework -f reconstruct -a dsp1 -v 2");
    // sleep(100);

    // SSDP_Configure(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "myapp1"), "BCH192_64800-DVBS2@9b2dc56", "Code Rate", "0x1000", 20);
    // SSDP_DeleteDevice();
    // cout<<"end testing"<<endl;
    //等待接听指令
    init();
    parse_args(argc, argv);
    SSDPServer server;
    if(!server.setup(IP, port, true)){
        exit(1);
    }

    server.do_epoll();
    return 1;
   
    return 0;
}
