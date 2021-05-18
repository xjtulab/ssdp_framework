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
    cout<<"zed dev"<<this->DEV_GetHandleName()<<" is starting"<<endl;
    if(send(sockfd, cmd.c_str(), cmd.size(),0) <= 0){
        printf("write failed ... \n");
        return SSDP_ERROR;
    }
}

SSDP_Result DeviceZED::DEV_Stop(){
    string cmd = "stop";
    cout<<"zed dev"<<this->DEV_GetHandleName()<<" is stoping"<<endl;
    if(send(sockfd, cmd.c_str(), cmd.size(),0) <= 0){
        printf("write failed ... \n");
        return SSDP_ERROR;
    }
}

SSDP_Result DeviceZED::DEV_Configure(string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie){
    string cmd = "config "+name+" "+value;
    cout<<"zed dev"<<this->DEV_GetHandleName()<<" is configing"<<endl;
    if(send(sockfd, cmd.c_str(), cmd.size(),0) <= 0){
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
    }	
}

std::string DeviceZED::DEV_Status_Qeury(){
    return "zed status";
};
SSDP_Result DeviceZED::DEV_Check(){
    return SSDP_OK;
}