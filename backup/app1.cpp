#include "base_app.h"
#include <iostream>
#include "SSDP_PRE_DATA.h"
using std::string;
using std::cout;
using std::endl;
//测试应用1
class APP1: public AppBase{
private:
    int pro1;
public:
    APP1(string name, SSDP_HandleID id, ssdp_app_functable* ft):AppBase(name,id, ft){}
    virtual SSDP_Result  APP_Start(){
        cout<<"APP1 started"<<std::endl;
        //ftable.start(1,1);
        return 0;
    }
    virtual SSDP_Result APP_Stop(){
        cout<<"APP1 stopped"<<std::endl;
        //ftable.stop(2,2);
        return 0;
    }
    virtual SSDP_Result APP_Initialize() {
        cout<<"APP1 init"<<endl;
        return 0;
    }
    virtual SSDP_Result APP_ReleaseObject(){
        cout<<"APP1 Release"<<endl;
        return 0;
    }
    virtual SSDP_Result APP_Write(SSDP_Message buffer, SSDP_Buffer_Size buffer_size){
        cout<<"APP1 write"<<endl;
        return 0;
    }
    virtual SSDP_Result APP_Read(SSDP_Message& buffer, SSDP_Buffer_Size buffer_size){
        cout<<"APP1 read"<<endl;
        return 0;
    }
    virtual SSDP_Result APP_Configure(SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_szie){
        cout<<"APP1 config"<<endl;
        return 0;
    }
    virtual SSDP_Result APP_Query (SSDP_Property_Name name, SSDP_Property_Value& value, SSDP_Buffer_Size value_size){
        cout<<"APP1 query"<<endl;
        return 0;
    }
};

extern "C" AppBase*  APP_Instance(string name, SSDP_HandleID id, ssdp_app_functable* ft) {
    return new APP1(name,id,ft);
}
extern "C" void APP_Destroy(AppBase* p) {
    delete p;
}
