#include "base_app.hpp"
#include <iostream>
using std::string;
using std::cout;
using std::endl;
//测试应用1
class APP1: public AppBase{
private:
    int pro1;
public:
    APP1(string name, int id, struct ssdp_app_functable ft):AppBase(name,id, ft){}
    virtual int  APP_start(){
        cout<<"APP1 started"<<std::endl;
        //ftable.start(1,1);
        return 0;
    }
    virtual int APP_stop(){
        cout<<"APP1 stopped"<<std::endl;
        //ftable.stop(2,2);
        return 0;
    }
    virtual int APP_Initialize() {
        cout<<"APP1 init"<<endl;
        return 0;
    }
    virtual int APP_ReleaseObject(){
        cout<<"APP1 Release"<<endl;
        return 0;
    }
    virtual int APP_Write(string buffer, int buffer_size){
        cout<<"APP1 write"<<endl;
        return 0;
    }
    virtual int APP_Read(string* buffer, int buffer_size){
        cout<<"APP1 read"<<endl;
        return 0;
    }
    virtual int APP_Configure(string name, string value, int value_szie){
        cout<<"APP1 config"<<endl;
        return 0;
    }
    virtual int APP_Query (string name, string* value, int value_size){
        cout<<"APP1 query"<<endl;
        return 0;
    }
};

extern "C" AppBase*  APP_Instance(string name, int id, struct ssdp_app_functable ft) {
    return new APP1(name,id,ft);
}
extern "C" void APP_Destroy(AppBase* p) {
    delete p;
}
