#include "SSDP_API.h"
#include "SSDP.h"
#include "SSDP_PRE_DATA.h"
#include <string>
#include "base_app.h"
#include <dlfcn.h>
#include <iostream>
#include <map>
using std::string;
using std::map;
using std::cout;
using std::endl;
//变量区
//sotable存放打开的动态库
//TODO 目前没考虑什么时间卸载动态库
map<string,sofiles*> sotable; 
//apptable存放启动的应用对象
map<SSDP_HandleID, app_objects> apptable;
//ssdp应用管理函数表
ssdp_app_functable app_functable;
//handleid管理
static SSDP_HandleID cur_id = 4;

//TODO 获取新应用的handleid，可考虑怎么优化ID的生成和分配
SSDP_HandleID get_new_id(){
    return cur_id++;
}
//获取目标so文件对象
sofiles* get_so(string sofilename){
    map<string,sofiles*>::iterator iter = sotable.find(sofilename);
    if (iter == sotable.end()){
        void* so_object = dlopen(sofilename.c_str(),RTLD_LAZY);
        APP_Instance_ptr create = (APP_Instance_ptr) dlsym(so_object, "APP_Instance");
        APP_Destroy_ptr destroy = (APP_Destroy_ptr) dlsym(so_object,"APP_Destroy");
        sofiles* new_so = new sofiles(so_object,create,destroy);
        sotable.insert(make_pair(sofilename,new_so));
        return new_so;
    }
    else{
        return iter->second;
    }
}

SSDP_HandleID SSDP_InstantiateApp(SSDP_HandleID fromid, string handlename, string sofile ){
    sofiles * targetso = get_so(sofile);
    AppBase* new_app = targetso->create(handlename,get_new_id(),&app_functable);
    app_objects a_o(new_app,targetso);
    apptable.insert(std::make_pair(new_app->APP_GetHandleID(),a_o));
    return new_app->APP_GetHandleID();
}

SSDP_Result SSDP_Start(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Start();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Stop(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Stop();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Initialize(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Initialize();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_ReleaseObject(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_ReleaseObject();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Write(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Message buffer, SSDP_Buffer_Size buffer_size){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Write(buffer,buffer_size);
        return buffer_size;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Read(SSDP_HandleID formid, SSDP_HandleID toid, SSDP_Message& buffer, SSDP_Buffer_Size buffer_size){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Read(buffer,buffer_size);
        return buffer_size;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_GetHandleName(SSDP_HandleID fromid, SSDP_HandleID toid, string& targetname){
    if (apptable.count(toid) != 0){
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_ValidateHandleID(SSDP_HandleID testid){
    if (apptable.count(testid) != 0){
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Configure(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Configure(name,value,value_size);
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Query(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size){
    if (apptable.count(toid) != 0){
        apptable[toid].app_ptr->APP_Query(name,value,value_size);
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_AbortApp(SSDP_HandleID fromid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid].so_ptr->destroy(apptable[toid].app_ptr);
        apptable.erase(toid);
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_show_cur_apps(){
    if (apptable.size() == 0){
        cout<<"no app now"<<endl;
    }
    auto iter = apptable.begin();
    while(iter != apptable.end()){
        cout<<iter->first<<" "<<iter->second.app_ptr->APP_GetHandleName()<<endl;
        ++iter;
    }
}

SSDP_Result SSDP_self_Init(){
    app_functable.instan = &SSDP_InstantiateApp;
    app_functable.start = &SSDP_Start;
    app_functable.stop = &SSDP_Stop;
    app_functable.init = &SSDP_Initialize;
    app_functable.release = &SSDP_ReleaseObject;
    app_functable.write = &SSDP_Write;
    app_functable.read = &SSDP_Read;
    app_functable.getname = &SSDP_GetHandleName;
    app_functable.validid = &SSDP_ValidateHandleID;
    app_functable.config = &SSDP_Configure;
    app_functable.query = &SSDP_Query;
    app_functable.abort = &SSDP_AbortApp;
    return 0;
}
