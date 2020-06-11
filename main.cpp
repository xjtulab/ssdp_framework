//
// Created by ljw on 6/9/20.
//
#include "base_app.hpp"
#include <iostream>

#include <dlfcn.h>
using std::cout;
using std::endl;
using std::cerr;
int SSDP_InstantiateApp (int fromid, std::string handlename, std::string filepath ){
    cout<<"SSDP_InstantiateApp"<<endl;
};
int SSDP_start (int fromid,int toid ){
    cout<<fromid<<endl;
};
int SSDP_stop (int fromid,int toid ){
    cout<<fromid<<endl;
};

struct ssdp_functable ssdp_funs;


int main() {
    //APP1TEST
    ssdp_funs.init = &SSDP_InstantiateApp;
    ssdp_funs.start = &SSDP_start;
    ssdp_funs.stop = &SSDP_stop;
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

}
