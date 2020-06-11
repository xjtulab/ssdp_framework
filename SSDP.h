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
typedef int (*SSDP_Initializeptr) (int fromid, int toid);
typedef int (*SSDP_ReleaseObjectptr) (int fromid, int toid);
typedef int (*SSDP_Writeptr) (int fromid, int toid, string buffer, int buffer_size);
typedef int (*SSDP_Readptr) (int formid, int toid, string* buffer, int buffer_size);
typedef int (*SSDP_GetHandleNameptr) (int fromid, int toid, string targetname);
typedef int (*SSDP_ValidateHandleIDptr) (int testid);
typedef int (*SSDP_Configureptr) (int fromid, int toid, string name, string value, int value_size);
typedef int (*SSDP_Queryptr) (int fromid, int toid, string name, string* value, int value_size);
typedef int (*SSDP_AbortAppptr) (int fromid, int toid);

//APP控制函数表
struct ssdp_app_functable{
    SSDP_InstantiateApp_ptr instan;
    SSDP_Start_ptr start;
    SSDP_Stop_ptr stop;
    SSDP_Initializeptr init;
    SSDP_ReleaseObjectptr release;
    SSDP_Writeptr write;
    SSDP_Readptr read;
    SSDP_GetHandleNameptr getname;
    SSDP_ValidateHandleIDptr validid;
    SSDP_Configureptr config;
    SSDP_Queryptr query;
    SSDP_AbortAppptr abort;
};
#endif //UNTITLED_SSDP_H
