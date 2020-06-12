//
// Created by ljw on 6/10/20.
//

#ifndef UNTITLED_SSDP_H
#define UNTITLED_SSDP_H
#include <string>
#include "SSDP_PRE_DATA.h"
using std::string;
// APP控制函数指针
typedef SSDP_HandleID (*SSDP_InstantiateApp_ptr) (SSDP_HandleID fromid, string handlename, string filepath );
typedef SSDP_Result (*SSDP_Start_ptr) (SSDP_HandleID fromid,SSDP_HandleID toid );
typedef SSDP_Result (*SSDP_Stop_ptr) (SSDP_HandleID fromid,SSDP_HandleID toid );
typedef SSDP_Result (*SSDP_Initialize_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid);
typedef SSDP_Result (*SSDP_ReleaseObject_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid);
typedef SSDP_Result (*SSDP_Write_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid, string buffer, SSDP_Buffer_Size buffer_size);
typedef SSDP_Result (*SSDP_Read_ptr) (SSDP_HandleID formid, SSDP_HandleID toid, string& buffer, SSDP_Buffer_Size buffer_size);
typedef SSDP_Result (*SSDP_GetHandleName_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid, string& targetname);
typedef SSDP_Result (*SSDP_ValidateHandleID_ptr) (SSDP_HandleID testid);
typedef SSDP_Result (*SSDP_Configure_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid, string name, string value, SSDP_Buffer_Size value_size);
typedef SSDP_Result (*SSDP_Query_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid, string name, string& value, SSDP_Buffer_Size value_size);
typedef SSDP_Result (*SSDP_AbortApp_ptr) (SSDP_HandleID fromid, SSDP_HandleID toid);

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
