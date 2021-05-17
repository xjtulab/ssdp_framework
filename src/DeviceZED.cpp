#include "devices/DeviceZED.h"
#include "zed_client.h"

DeviceZED::DeviceZED(string name, SSDP_HandleID id): DeviceBase(name, id){
    cout<<"zed device "<<this->handle_name<<" inited"<<endl;
}

DeviceZED::~DeviceZED(){
    cout<<"deleteing zed device"<<endl;
}

SSDP_Result DeviceZED::DEV_Start(){
    string cmd = "start 0x03 0x04 0x01";
    string cmd1 = "config 0x03 0x05 0x01";
    if(send(sockfd, cmd.c_str(), cmd.size(),0) <= 0){
        printf("write failed ... \n");
        return SSDP_ERROR;
    }
    if(send(sockfd, cmd1.c_str(), cmd1.size(),0) <= 0){
        printf("write failed ... \n");
        return SSDP_ERROR;
    }
}

void DeviceZED::DEV_SetPub(string ip, string port, string topic_name, string session_key){
    cout<<"creating socket"<<ip<<" "<<" "<<port<<" "<<endl; 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    uint16_t port_int = atoi(( char*)port.c_str());
    serv_addr.sin_port = htons(port_int);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("connect failed ... \n");
        // return -1;
    }
	// return 1;	
}

std::string DeviceZED::DEV_Status_Qeury(){
    return "zed status";
};
SSDP_Result DeviceZED::DEV_Check(){
    return SSDP_OK;
}