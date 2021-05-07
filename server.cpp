#include "SSDPServer.hpp"
#include <thread>

#define SSDP_SERVER_IP "192.168.1.164"
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

int main(int argc, char **argv){
    init();
    parse_args(argc, argv);
    SSDPServer server;
    if(!server.setup(IP, port, true)){
        exit(1);
    }
    // std::thread recv_thread(recv_from_upper_pc, &server);
    // recv_thread.detach();

    server.do_epoll();

    return 0;
}
