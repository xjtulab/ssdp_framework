#include "base_app.hpp"
#include <iostream>
using std::string;
using std::cout;
using std::endl;
//测试应用1
class APP2: public AppBase{
private:
    int pro1;
public:
    APP2(string name, int id, struct ssdp_app_functable* ft):AppBase(name,id, ft){}
    virtual int  APP_Start(){
        cout<<"APP2 started"<<std::endl;
        ftable->start(APP_GetHandleID(),1);
        return 0;
    }
    virtual int APP_Stop(){
        cout<<"APP2 stopped"<<std::endl;
        //ftable.stop(2,2);
        return 0;
    }
    virtual int APP_Initialize() {
        cout<<"APP2 init"<<endl;
        return 0;
    }
    virtual int APP_ReleaseObject(){
        cout<<"APP2 Release"<<endl;
        return 0;
    }
    virtual int APP_Write(string buffer, int buffer_size){
        cout<<"APP2 write"<<endl;
        return 0;
    }
    virtual int APP_Read(string* buffer, int buffer_size){
        cout<<"APP2 read"<<endl;
        return 0;
    }
    virtual int APP_Configure(string name, string value, int value_szie){
        cout<<"APP2 config"<<endl;
        return 0;
    }
    virtual int APP_Query (string name, string* value, int value_size){
        cout<<"APP2 query"<<endl;
        return 0;
    }
};

extern "C" AppBase*  APP_Instance(string name, int id, struct ssdp_app_functable* ft) {
    return new APP2(name,id,ft);
}
extern "C" void APP_Destroy(AppBase* p) {
    delete p;
}
