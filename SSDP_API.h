#ifndef UNTITLED_SSDP_API_H
#define UNTITLED_SSDP_API_H
#include <string>
#include "base_app.hpp"
#include <dlfcn.h>
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


int SSDP_InstantiateApp(int fromid, string handlename, string filepath );
int SSDP_Start(int fromid,int toid );
int SSDP_Stop(int fromid,int toid );
int SSDP_Initialize(int fromid, int toid);
int SSDP_Configure(int fromid, int toid, string name, string value, int value_size);
int SSDP_Query(int fromid, int toid, string name, string* value, int value_size);
int SSDP_AbortApp(int fromid, int toid);
int SSDP_show_cur_apps();
int SSDP_self_Init();
#endif