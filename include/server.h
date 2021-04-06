#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include<iostream>
#include<fstream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstdio>
#include<string.h>
#include<unistd.h>
#include<string>
#include<limits.h>
#include<stdlib.h>
#define MAX_BUFFER_SIZE 1024

using namespace std;

struct MesRecieved
{
    /* data */
    int flag;           //flag: 0->recieve instructions; 1->recieve file
    string content;     //flag==0: instructions contents;flag==1: file path
};

class Server
{
private:
    
    // 配置文件保存的固定目录
    const string directory = "/home/linjw/ssdp_work/";
    /* data */
    int server_socketfd;
    char* serverIP;
    int serverPort;
    struct sockaddr_in server_sockaddr;

    char recv_buf[MAX_BUFFER_SIZE];
    char send_buf[MAX_BUFFER_SIZE];

    MesRecieved recvFile(const int& client_socketfd, string filename);
    MesRecieved recvIns(const int& client_socketfd);
public:
    Server(/* args */char* ip, int port);
    ~Server();
    bool setup();
    int do_accept(sockaddr * __addr, socklen_t *__addr_len);
    MesRecieved do_recv(const int& client_socketfd);
    void do_send(const int& client_socketfd, string reply);
    void do_shutdown();
};


#endif
