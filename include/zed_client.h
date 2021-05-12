#ifndef ZED_CLIENT_H
#define ZED_CLIENT_H
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>



int init_client(char* ip, int port);

void send_data(char* message, int len);

#endif
