//
// Created by ljw on 6/10/20.
//

#ifndef UNTITLED_SSDP_H
#define UNTITLED_SSDP_H
#include <string>
using std::string;
// APP控制函数指针
typedef int (*SSDP_InstantiateApp_ptr) (int fromid, string handlename, string filepath );
typedef int (*SSDP_Start_ptr) (int fromid,int toid );
typedef int (*SSDP_Stop_ptr) (int fromid,int toid );
typedef int (*SSDP_Initialize_ptr) (int fromid, int toid);
typedef int (*SSDP_ReleaseObject_ptr) (int fromid, int toid);
typedef int (*SSDP_Write_ptr) (int fromid, int toid, string buffer, int buffer_size);
typedef int (*SSDP_Read_ptr) (int formid, int toid, string* buffer, int buffer_size);
typedef int (*SSDP_GetHandleName_ptr) (int fromid, int toid, string* targetname);
typedef int (*SSDP_ValidateHandleID_ptr) (int testid);
typedef int (*SSDP_Configure_ptr) (int fromid, int toid, string name, string value, int value_size);
typedef int (*SSDP_Query_ptr) (int fromid, int toid, string name, string* value, int value_size);
typedef int (*SSDP_AbortApp_ptr) (int fromid, int toid);

//APP控制函数表
struct ssdp_app_functable{
    SSDP_InstantiateApp_ptr instan;
    SSDP_Start_ptr start;
    SSDP_Stop_ptr stop;
    SSDP_Initialize_ptr init;
    SSDP_ReleaseObject_ptr release;
    SSDP_Write_ptr write;
    SSDP_Read_ptr read;
    SSDP_GetHandleName_ptr getname;
    SSDP_ValidateHandleID_ptr validid;
    SSDP_Configure_ptr config;
    SSDP_Query_ptr query;
    SSDP_AbortApp_ptr abort;
};
#endif //UNTITLED_SSDP_H
