#include "SSDP_API.h"
#include "SSDP.h"
#include "SSDP_PRE_DATA.h"
#include <string>
#include "base_app.h"
#include <dlfcn.h>
#include <iostream>
#include <map>
#include <memory>
#include <set>
using std::string;
using std::map;
using std::cout;
using std::endl;
using std::set;
//变量区

//apptable存放启动的应用对象
map<SSDP_HandleID, std::shared_ptr<AppBase>> apptable;
//ssdp应用管理函数表
ssdp_app_functable app_functable;

//HandleID管理相关数据
//TODO 目前假设32位的id够一次运行中使用

static SSDP_HandleID cur_id = BEGIN_ID;
set<SSDP_HandleID> id_in_use;

//TODO 获取新应用的handleid，可考虑怎么优化ID的生成和分配
SSDP_HandleID SSDP_GetNewHandleID(){
    
    id_in_use.insert(cur_id);
    SSDP_HandleID res = cur_id++;
    return res;
}


//创建应用实例
SSDP_HandleID SSDP_InstantiateApp(SSDP_HandleID fromid, string handlename, string filepath ){
    auto new_app = std::make_shared<AppBase>(handlename, SSDP_GetNewHandleID(),&app_functable);
    //cout<<new_app.use_count()<<endl;
    apptable.insert(std::make_pair(new_app->APP_GetHandleID(),new_app));
    //cout<<new_app.use_count()<<endl;
    //TODO 进行应用属性配置，启动等
    //new_app.use_count();
    return new_app->APP_GetHandleID();
}

SSDP_Result SSDP_Start(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Start();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Stop(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Stop();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Initialize(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Initialize();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_ReleaseObject(SSDP_HandleID formid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_ReleaseObject();
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Write(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Message buffer, SSDP_Buffer_Size buffer_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Write(buffer,buffer_size);
        return buffer_size;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Read(SSDP_HandleID formid, SSDP_HandleID toid, SSDP_Message& buffer, SSDP_Buffer_Size buffer_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Read(buffer,buffer_size);
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
        apptable[toid]->APP_Configure(name,value,value_size);
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Query(SSDP_HandleID fromid, SSDP_HandleID toid, SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Query(name,value,value_size);
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_AbortApp(SSDP_HandleID fromid, SSDP_HandleID toid){
    if (apptable.count(toid) != 0){
        //cout<<apptable[toid].use_count()<<endl;
        //delete apptable[toid];
        //auto app = apptable[toid];
        //cout<<apptable[toid].use_count()<<endl;
        //delete apptable[toid];
        apptable.erase(toid);
        //cout<<ad->APP_GetHandleID()<<endl;
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
        cout<<iter->first<<" "<<iter->second->APP_GetHandleName()<<endl;
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
