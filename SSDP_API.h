#ifndef UNTITLED_SSDP_API_H
#define UNTITLED_SSDP_API_H
#include <string>
#include "base_app.hpp"
#include <dlfcn.h>
using std::string;

struct sofiles{
    void* so_boject;
    APP_Instance_ptr create;
    APP_Destroy_ptr destroy;
};


int SSDP_InstantiateApp(int fromid, string handlename, string filepath );
int SSDP_Start(int fromid,int toid );
int SSDP_Stop(int fromid,int toid );
int SSDP_Initialize(int fromid, int toid);
int SSDP_Configure(int fromid, int toid, string name, string value, int value_size);
int SSDP_Query(int fromid, int toid, string name, string* value, int value_size);
#endif