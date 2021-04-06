#ifndef UNTITLED_SSDP_API_H
#define UNTITLED_SSDP_API_H
#include <string>
#include "base_app.h"
#include <dlfcn.h>
#include "SSDP_PRE_DATA.h"
using std::string;

#define BEGIN_ID 4


//ID管理相关API
SSDP_Result SSDP_ValidateHandleID(SSDP_HandleID testid);
SSDP_Result SSDP_GetHandleName(SSDP_HandleID fromid, SSDP_HandleID toid, string& targetname);
SSDP_HandleID SSDP_HandleRequest(SSDP_HandleID fromid, const string& targetname);
bool SSDP_IsOK(SSDP_Result result);
//应用控制相关API
SSDP_HandleID SSDP_InstantiateApp(SSDP_HandleID fromid, string handlename, string config_file_path);
SSDP_Result SSDP_Start(SSDP_HandleID fromid,SSDP_HandleID toid );
SSDP_Result SSDP_Stop(SSDP_HandleID fromid,SSDP_HandleID toid );
SSDP_Result SSDP_Initialize(SSDP_HandleID fromid, SSDP_HandleID toid);
SSDP_Result SSDP_Configure(SSDP_HandleID fromid, SSDP_HandleID toid,string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size);
SSDP_Result SSDP_Query(SSDP_HandleID fromid, SSDP_HandleID toid,int comp_id, SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size);
SSDP_Result SSDP_AbortApp(SSDP_HandleID fromid, SSDP_HandleID toid);
SSDP_Result SSDP_Write(SSDP_HandleID fromid, SSDP_HandleID toid,int comp_id, string buffer, SSDP_Buffer_Size buffer_size);
SSDP_Result SSDP_Read(SSDP_HandleID formid, SSDP_HandleID toid,int comp_id, string& buffer, SSDP_Buffer_Size buffer_size);
SSDP_Result SSDP_show_cur_apps();
SSDP_Result SSDP_self_Init();

//设备相关API
SSDP_HandleID SSDP_InstantiateDevice(SSDP_HandleID fromid, string handlename, string config_file_path);
#endif