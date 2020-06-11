#include "SSDP_API.h"
#include "SSDP.h"
#include <string>
#include "base_app.hpp"
#include <dlfcn.h>
using std::string;

struct sofiles sotable[128];
static AppBase* apptable[128];
static int app_nums = 1;

int SSDP_InstantiateApp(int fromid, string handlename, string filepath ){
    sotable[app_nums].so_boject = dlopen(filepath.c_str(), RTLD_LAZY);
    
}