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

SSDP_Result SSDP_ValidateHandleID(SSDP_HandleID testid){
    if(testid == SSDP_ERROR_QUEUE || testid == SSDP_FATAL_QUEUE || testid == SSDP_WARNING_QUEUE){
        return SSDP_OK;
    }
    if(testid == SSDP_OE_HANDLE_ID){
        return SSDP_OK;
    }
    if (id_in_use.count(testid) != 0){
        return SSDP_OK;
    }else{
        return SSDP_ERROR;
    }
}

SSDP_HandleID SSDP_HandleRequest(SSDP_HandleID fromid, const string& targetname){
    //要求的名字不存在则返回SSDP_ERROR
    SSDP_HandleID res = SSDP_ERROR;
    auto iter = apptable.begin();
    while(iter != apptable.end()){
        if(iter->second->APP_GetHandleName() ==  targetname){
            res = iter->second->APP_GetHandleID();
        }
        ++iter;
    }
    return res;
}

SSDP_Result SSDP_GetHandleName(SSDP_HandleID fromid, SSDP_HandleID toid, string& targetname){
    //TODO ID不存在返回什么值？
    string res_str = "EMPTY";
    SSDP_Result res = SSDP_ERROR;
    if(toid == SSDP_OE_HANDLE_ID){
        res = SSDP_OK;
        res_str = SSDP_OE_HANDLE_NAME;
    }
    if(apptable.count(toid) >0){
        res_str = apptable[toid]->APP_GetHandleName();
        res = SSDP_OK;
    }
    targetname = res_str;
    return res;
}

bool SSDP_IsOK(SSDP_Result result){
    if (result >=0) return true;
    else return false;
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

SSDP_Result SSDP_Write(SSDP_HandleID fromid, SSDP_HandleID toid,int comp_id, SSDP_Message buffer, SSDP_Buffer_Size buffer_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Write(comp_id, buffer,buffer_size);
        return buffer_size;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Read(SSDP_HandleID formid, SSDP_HandleID toid,int comp_id, SSDP_Message& buffer, SSDP_Buffer_Size buffer_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Read(comp_id,buffer,buffer_size);
        return buffer_size;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Configure(SSDP_HandleID fromid, SSDP_HandleID toid,int comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Configure(comp_id,name,value,value_size);
        return 0;
    }
    else{
        return -1;
    }
}

SSDP_Result SSDP_Query(SSDP_HandleID fromid, SSDP_HandleID toid,int comp_id, SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size){
    if (apptable.count(toid) != 0){
        apptable[toid]->APP_Query(comp_id,name,value,value_size);
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
        id_in_use.erase(toid);
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
