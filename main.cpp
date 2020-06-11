//
// Created by ljw on 6/9/20.
//
#include "base_app.hpp"
#include <iostream>
#include "SSDP.h"
#include "SSDP_API.h"
using std::cout;
using std::endl;
using std::cerr;


int main() {
    //APP1TEST
    int ssdp_init_result = SSDP_self_Init();
    if (ssdp_init_result != 0){
        cout<<"ssdp init failed"<<endl;
        return 1;
    }
    SSDP_show_cur_apps();
    int appid = SSDP_InstantiateApp(0,"myapp1","./app1.so");
    int appid1 = SSDP_InstantiateApp(0,"myapp2","./app2.so");
    SSDP_Start(0,appid1);
    SSDP_Stop(0,appid1);
    SSDP_show_cur_apps();
    SSDP_AbortApp(0,appid);
    SSDP_show_cur_apps();
    SSDP_AbortApp(0,appid1);
    SSDP_show_cur_apps();
    return 0;
    /*
    void *app1 = dlopen("./app1.so",RTLD_LAZY);
    if (!app1){
        cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }
    dlerror();
    create_t* create_app1 = (create_t*) dlsym(app1,"create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }
    destroy_t* destroy_app1 = (destroy_t*) dlsym(app1,"destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }
    callfunc callapp1 = (callfunc) dlsym(app1,"call");
    //APP1TEST

    TESTAPP* myapp1 = create_app1(ssdp_funs);
    myapp1->APP_start();
    myapp1->APP_stop();

    destroy_app1(myapp1);
    dlclose(app1);
    */
}
