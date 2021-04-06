
#include "server.h"

Server::Server(char* ip, int port):serverIP(ip),serverPort(port){}
Server::~Server()
{
    close(server_socketfd);
    printf("socket closed\n");
}

void Server::do_shutdown()
{
    shutdown(server_socketfd, SHUT_RDWR);
}

bool Server::setup()
{
    server_socketfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_sockaddr, 0, sizeof(server_sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(serverPort);
    server_sockaddr.sin_addr.s_addr = inet_addr(serverIP);

    if (bind(server_socketfd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1)
    {
        printf("bind error\n");
        return false;
    }

    if (listen(server_socketfd, 5) == -1)
    {
        printf("listen error\n");
        return false;
    }
    return true;

    
}

int Server::do_accept(sockaddr * __addr, socklen_t *__addr_len)
{
    
    return accept(server_socketfd, __addr, __addr_len);
    
}


MesRecieved Server::do_recv(const int& client_socketfd)
{
    memset(&recv_buf, 0, sizeof(recv_buf));
    int length = recv(client_socketfd, recv_buf, 2, 0);

    // flag's length
    unsigned short flag_len = 0;
    if (length == 2)
    {
        flag_len = (unsigned char)recv_buf[0];
        flag_len = flag_len<<8;
        unsigned short t = (unsigned char)recv_buf[1];
        flag_len = flag_len + t;
     }
    memset(&recv_buf, 0, 2);
    // recv flag
    length = recv(client_socketfd, recv_buf, flag_len, 0);
    unsigned short flag = 0;
    if (length == flag_len)
    {
        flag = recv_buf[0];
        if (flag == 0)
        {
            return recvIns(client_socketfd);

        }else if (flag == 1)
        {
            char* name = new char[flag_len];
            char* recv_filename = recv_buf + 1;
            memcpy(name, recv_filename, flag_len-1);
            name[flag_len-1] = '\0';
            string filename(name);
            return recvFile(client_socketfd, filename);
        }
    }
}

MesRecieved Server::recvIns(const int& client_socketfd)
{
    memset(&recv_buf, 0, sizeof(recv_buf));
    int length = recv(client_socketfd, recv_buf, 3, 0);

    unsigned int ins_len = 0;
    if (length == 3)
    {
        for (size_t i = 0; i < length; i++)
        {
            ins_len = ins_len + (unsigned char)recv_buf[i];
            if (i<2)
            {
                ins_len = ins_len<<8;
            }
        }
    }
    memset(&recv_buf, 0, 3);
    length = recv(client_socketfd, recv_buf, ins_len, 0);
    if (length == ins_len)
    {
        char* ins = new char[ins_len+1];
        memcpy(ins, recv_buf, ins_len);
        ins[ins_len] = '\0';
        MesRecieved mes;
        mes.flag = 0;
        mes.content = ins;
        
        return mes;
    }
}

MesRecieved Server::recvFile(const int& client_socketfd, string filename)
{
    memset(&recv_buf, 0, sizeof(recv_buf));
    int length = recv(client_socketfd, recv_buf, 3, 0);

    int file_len = 0;
    if (length == 3)
    {
        for (size_t i = 0; i < length; i++)
        {
            file_len = file_len + (unsigned char)recv_buf[i];
            if (i<2)
            {
                file_len = file_len<<8;
            }
        }
    }

    string path;
    path = directory + filename;

    FILE *fp = fopen(path.c_str(), "wb");
    if (fp != NULL)
    {
        /* code */
        int pos = 0;
        int count = 0;
        memset(&recv_buf, 0, sizeof(recv_buf));
        while (file_len-count>0)
        {
            pos = recv(client_socketfd, recv_buf, sizeof(recv_buf), 0);
            fwrite(recv_buf, 1, pos, fp);
            count = count + pos;
        }
        fclose(fp);
        MesRecieved mes;
        mes.flag = 1;
        mes.content = path;

        return mes;
    }
}

void Server::do_send(const int& client_socketfd, string reply)
{
    send(client_socketfd, reply.c_str(), reply.size(), 0);
}