#ifndef UNTITLED_SSDP_API_H
#define UNTITLED_SSDP_API_H
#include <string>
#include "base_app.hpp"
#include <dlfcn.h>
#include "SSDP_pre_data.h"
using std::string;

struct sofiles{
    void* so_object;
    APP_Instance_ptr create;
    APP_Destroy_ptr destroy;
    sofiles(void* so,APP_Instance_ptr cre, APP_Destroy_ptr des){
        so_object = so;
        create = cre;
        destroy = des;
    }
};

struct app_objects{
    AppBase* app_ptr;
    struct sofiles* so_ptr;
    app_objects(AppBase* app,struct sofiles* so){
        app_ptr = app;
        so_ptr = so;
    }
    app_objects(){
        app_ptr = NULL;
        so_ptr =NULL;
    }
};


SSDP_HandleID SSDP_InstantiateApp(SSDP_HandleID fromid, string handlename, string filepath );
SSDP_Result SSDP_Start(SSDP_HandleID fromid,SSDP_HandleID toid );
SSDP_Result SSDP_Stop(SSDP_HandleID fromid,SSDP_HandleID toid );
SSDP_Result SSDP_Initialize(SSDP_HandleID fromid, SSDP_HandleID toid);
SSDP_Result SSDP_Configure(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size);
SSDP_Result SSDP_Query(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size);
SSDP_Result SSDP_AbortApp(SSDP_HandleID fromid, SSDP_HandleID toid);
SSDP_Result SSDP_show_cur_apps();
SSDP_Result SSDP_self_Init();
#endif