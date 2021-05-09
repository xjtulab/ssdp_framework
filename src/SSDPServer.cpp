
#include "SSDPServer.hpp"
#include "SSDP_API.h"

std::string SSDPServer::directory;

void SSDPServer::set_nonblock(int sockfd){
    int opts = fcntl(sockfd, F_GETFL);
    exit_if(opts < 0, "fcntl(F_GETFL): ");
    opts |= O_NONBLOCK;
    exit_if(fcntl(sockfd, F_SETFL, opts) < 0, "fcntl(F_SETFL): ");
}

void SSDPServer::set_default_directory(){
    char *path = get_current_dir_name();
    std::string dir(path);
    SSDPServer::directory = dir;
}

void SSDPServer::set_directory(char *path){
    std::string dir(path);
    SSDPServer::directory = dir;    
}

SSDPServer::SSDPServer(){}
SSDPServer::~SSDPServer()
{
    close(server_socketfd);
    printf("Server socket closed\n");
}

bool SSDPServer::setup(char* ip, int port, bool isblock)
{
    if((server_socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        fprintf(stderr, "Create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }
    if(!isblock){
        set_nonblock(server_socketfd);
    }

    memset(&server_sockaddr, 0, sizeof(server_sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = inet_addr(ip);

    // set reuse address
    int on = 1;
 
    if (setsockopt(server_socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1){
        fprintf(stderr, "Set reuse addr error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }

    if (bind(server_socketfd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1)
    {
        fprintf(stderr, "Bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }

    if (listen(server_socketfd, FDS_SIZE) == -1)
    {
        fprintf(stderr, "Listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }
    printf("Create SSDP Server: success!\n");
    return true;
}

void SSDPServer::do_epoll(){
    struct epoll_event events[MAX_EVENTS];
    exit_if((epoll_fd = epoll_create(FDS_SIZE)) == -1, "create epoll error: ");

    add_event(server_socketfd, EPOLLIN);

    while(true){
        int ret = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for(int i=0;i<ret;i++){
            int fd = events[i].data.fd;
            if(fd == server_socketfd && events[i].events & EPOLLIN){
                do_accept();
            }else if(events[i].events & EPOLLIN){
                handle_message(fd);
            }else{
                del_event(fd, EPOLLIN);
                close(fd);
            }
        }
    }
}

void SSDPServer::add_event(int fd, int state) const{
    struct epoll_event ev{};
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

void SSDPServer::del_event(int fd, int state) const{
    struct epoll_event ev{};
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
}

int SSDPServer::do_accept(){
    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    int clientfd = accept(server_socketfd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(clientfd < 0){
        fprintf(stderr, "Accept socket error: %s(errno %d)\n", strerror(errno), errno);
        return -1;
    }else{
        fprintf(stdout, "Accept a new client with fd:%d->%s:%d\n", clientfd, inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        // set_nonblock(clientfd);
        add_event(clientfd, EPOLLIN);
    }
    return clientfd;
}

void SSDPServer::handle_message(int fd){
    MesRecieved mes = do_recv(fd);
    std::string reply;
    if(mes.flag >= 0){
        if(mes.flag == 0){
            //TODO 指令处理流程
            printf("Recieve Instruction: %s\n", mes.content.c_str());
            reply = "Recieve Instruction: " + mes.content + " success!";

        }else if(mes.flag == 1){
            //TODO 文件处理流程
            printf("Recieve Configuration file: %s\n", mes.content.c_str());
            reply = "Recieve Configuration file: " + mes.content + " success!";
        }else if(mes.flag == 2){
            printf("Recieve Device Status Request\n");
            reply = SSDP_DeviceStatus();
        }
        do_send(fd, reply);
    }
}

MesRecieved SSDPServer::do_recv(const int& client_socketfd)
{
    memset(recv_buf, 0, sizeof(recv_buf));
    ssize_t length = recv(client_socketfd, recv_buf, 2, 0);

    exit_if(length < 0, "Recieve flag error: ");
    if(length == 0){
        del_event(client_socketfd, EPOLLIN);
        close(client_socketfd);
        return MesRecieved(-1);
    }
    // std::cout<<"-----------Recieve flag-------------------"<<std::endl;

    // short flag = (short) recv_buf[0];
    short flag = 0;
    for(int i=0; i<2; i++){
        flag = flag << 1;
        flag += (short)recv_buf[i];
    }
    if(flag == 0){
        return recvIns(client_socketfd);
    }else if(flag == 1){
        memset(recv_buf, 0, sizeof(recv_buf));
        ssize_t l = recv(client_socketfd, recv_buf, MAX_FILENAME_LENGTH, 0);
        exit_if(l < 0, "Recieve filename error: ");

        char *name = new char[MAX_FILENAME_LENGTH];
        memcpy(name, recv_buf, MAX_FILENAME_LENGTH);
        std::string filename(name);
        // std::cout<<"-----------filename---------------: "<<filename<<std::endl;

        memset(recv_buf, 0, sizeof(recv_buf));
        l = recv(client_socketfd, recv_buf, MAX_FILE_LENGTH_BYTE, 0);
        exit_if(l < 0, "Recieve filesize error: ");
        file_size filesize = 0;
        for(int i=0;i<8;i++){
            filesize = filesize << 8;
            filesize += (unsigned char)recv_buf[i];
        }
        // std::cout<<"----------------filesize-----------------"<<filesize<<std::endl;
        return recvFile(client_socketfd, filename, filesize);
    }else if(flag == 2){
        return MesRecieved(2);
    }
    return MesRecieved(-1);
}

MesRecieved SSDPServer::recvIns(const int& client_socketfd)
{
    memset(recv_buf, 0, sizeof(recv_buf));
    ssize_t len = recv(client_socketfd, recv_buf, sizeof(recv_buf), 0);
    exit_if(len < 0 ,"Recieve instruction error");
    if(len == 0){
        del_event(client_socketfd, EPOLLIN);
        close(client_socketfd);
        return MesRecieved(-1);
    }
    // std::cout<<"-----------recieve ins------------: "<<std::endl;
    char *str = new char[len+1];
    memcpy(str, recv_buf, len);
    str[len] = '\0';
    std::string ins(str);
    return MesRecieved(0, ins);
}

MesRecieved SSDPServer::recvFile(const int& client_socketfd, std::string filename, file_size n)
{
    std::string path;
    path = directory + "/" + filename;

    FILE *fp = fopen(path.c_str(), "wb");
    if (fp != NULL){
        int pos = 0;
        int count = 0;
        while (n - count > 0){
            memset(recv_buf, 0, sizeof(recv_buf));
            pos = recv(client_socketfd, recv_buf, sizeof(recv_buf), 0);
            exit_if(pos < 0, "Recieve file error\n");
            if(pos == 0){
                del_event(client_socketfd, EPOLLIN);
                close(client_socketfd);
                return MesRecieved(-1);
            }
            fwrite(recv_buf, 1, pos, fp);
            count += pos;
        }
        fclose(fp);

        return MesRecieved(1, path);
    }
    return MesRecieved(-1);
}

void SSDPServer::do_send(const int& client_socketfd, std::string reply)
{
    size_t len = reply.size();
    memset(send_buf, 0, sizeof(send_buf));
    send_buf[0] = len>>8;
    send_buf[1] = len;
    send(client_socketfd, send_buf,2,0);
    send(client_socketfd, reply.c_str(), reply.size(), 0);
}