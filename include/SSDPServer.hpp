#ifndef SSDPSERVER_H
#define SSDPSERVER_H

#include<arpa/inet.h>
#include<iostream>
#include<string>
#include<string.h>
#include<fstream>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<cstdio>
#include<cerrno>
#include<unistd.h>
#include<limits.h>
#include<stdlib.h>
#define MAX_FILENAME_LENGTH 255
#define MAX_FILE_LENGTH_BYTE 8
#define MAX_BUFFER_SIZE 1024
#define FDS_SIZE 5
#define MAX_EVENTS 5

typedef unsigned long long file_size;

#define exit_if(r, ...)                                                                                   \
    if(r){                                                                                                \
        fprintf(stderr, __VA_ARGS__);                                                                     \
        fprintf(stderr, "%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); \
        exit(1);                                                                                          \
    }

struct MesRecieved
{
    /* data */
    int flag;           //flag: 0->recieve instructions; 1->recieve file
    std::string content;     //flag==0: instructions contents;flag==1: file path
    MesRecieved():flag(-1), content(""){}
    MesRecieved(int f):flag(f), content(""){}
    MesRecieved(int f, std::string con):flag(f), content(con){}
};

class SSDPServer
{
private:

    static std::string directory;
    /* data */
    int server_socketfd;
    int epoll_fd;
    char* serverIP;
    int serverPort;
    struct sockaddr_in server_sockaddr;
    // struct sockaddr_in client_sockaddr;

    char recv_buf[MAX_BUFFER_SIZE];
    char send_buf[MAX_BUFFER_SIZE];

    MesRecieved recvFile(const int& client_socketfd, std::string filename, file_size n);
    MesRecieved recvIns(const int& client_socketfd);
    MesRecieved do_recv(const int& client_socketfd);

    void handle_message(int fd);
    int do_accept();
    void do_send(const int& client_socketfd, std::string reply);

    void add_event(int fd, int state) const;
    void del_event(int fd, int state) const;

    void set_nonblock(int sockfd);

public:
    SSDPServer();
    ~SSDPServer();
    bool setup(char* ip, int port, bool isblock);
    static void set_directory(char *path);
    static void set_default_directory();
    void do_epoll();
    
};

#endif