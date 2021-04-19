//
// Created by ljw on 6/9/20.
//
//#include "base_app.h"

#include"server.h"
#include<thread>
#include <iostream>
#include "SSDP.h"
#include "SSDP_API.h"
#include "SSDP_LOG.h"
#include "commandprocess.h"
using std::cout;
using std::endl;
using std::cerr;

#define PORT 8080
#define IP "192.168.1.111"
CmdProcess cmdprocesser;
void start_routine(struct thread_params *params);

// 线程执行的函数参数
struct thread_params
{
    int fd;
    Server *server;
};

void start_routine(struct thread_params *params)
{
    int client_sockfd = params->fd;

    while (true)
    {
        MesRecieved mes= params->server->do_recv(client_sockfd);
        if (mes.flag == 0)
        {
            /* code */
            cout<<"recieve instructions: "<<mes.content<<endl;
            cmdprocesser.ReceiveCommand(mes.content);
        }else if (mes.flag == 1)
        {
            /* code */
            cout<<"recieve file: path-> "<<mes.content<<endl;
        }

        string reply = "recieve file success";
        params->server->do_send(client_sockfd, reply);

    }
    printf("client closed\n");
    close(client_sockfd);
}


int main() {
    // CmdProcess cmdprocesser;
    // Server *server = new Server(const_cast<char*>(IP), PORT);
    // struct thread_params params;
    // params.server = server;
    // struct sockaddr_in client_sockaddr;
    // socklen_t length = sizeof(client_sockaddr);

    // if(server->setup()){
    //     cout<<"setup success"<<endl;
    // }

    // while (true)
    // {

    //     params.fd = server->do_accept((struct sockaddr *)&client_sockaddr, &length);
    //     if (params.fd < 0)
    //     {
    //         printf("accept error\n");
    //         break;
    //     }else{
    //         printf("Accept a new client from fd:%d->%s:%d\n",params.fd,inet_ntoa(client_sockaddr.sin_addr),client_sockaddr.sin_port);
    //     }
        
    //     thread data_read(start_routine, &params);
    //     data_read.detach();
    // }
    // server->do_shutdown();
    // delete server;
    // server = NULL;
    // params.server = NULL;
    // return 0;


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
        return 1;
    }
    SSDP_show_cur_apps();

    // 1、读取设备配置文件，创建设备实例
    int devid = SSDP_InstantiateDevice(0, "fpga1", "a.xml");
    int devid1 = SSDP_InstantiateDevice(0, "dsp1", "a.xml");
    int devid2 = SSDP_InstantiateDevice(0, "dsp2", "a.xml");
    // int appid = SSDP_InstantiateApp(0,"myapp1","myapp1.xml");

    // 2、启动和显控界面的连接，准备接收指令
    CmdProcess cmdprocesser;
    Server *server = new Server(const_cast<char*>(IP), PORT);
    struct thread_params params;
    params.server = server;
    struct sockaddr_in client_sockaddr;
    socklen_t length = sizeof(client_sockaddr);

    if(server->setup()){
        cout<<"network setup success"<<endl;
    }

    while (true)
    {
        params.fd = server->do_accept((struct sockaddr *)&client_sockaddr, &length);
        if (params.fd < 0)
        {
            printf("accept error\n");
            break;
        }else{
            printf("Accept a new client from fd:%d->%s:%d\n",params.fd,inet_ntoa(client_sockaddr.sin_addr),client_sockaddr.sin_port);
        }
        
        thread data_read(start_routine, &params);
        data_read.detach();
    }
    server->do_shutdown();
    delete server;
    server = NULL;
    params.server = NULL;
    return 0;
    
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
