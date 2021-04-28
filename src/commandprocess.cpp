#include "commandprocess.h"
#include "SSDP_PRE_DATA.h"
#include "SSDP_API.h"
#include<vector>
using std::cout;
using std::endl;
using std::vector;



// static const string EnumStrings[] = { "start", "stop", "config" };


CmdProcess::CmdProcess(){
    par = new parser;
    //TODO 指令格式的设计待定
    par->add<string>("source", 's', "Command source", true);
    par->add<string>("target", 't', "Command target", true);
    par->add<string>("function", 'f',"Function to be called",true);
    par->add<string>("APP_name",'a',"APP Name", true);
    par->add<string>("comp_id",'c',"Component ID", false);
    par->add<string>("property_name",'n',"Property Name", false);
    par->add<string>("property_value",'v',"Property Value",false);
    par->set_program_name("SSDP");
}

SSDP_Result CmdProcess::ReceiveCommand(const string& cmd){
    bool ok = par->parse(cmd);
    if(!ok){
        cout<<"cmd error"<<endl;
        return SSDP_ERROR;
    }
    //TODO 加上ID检测，和功能检测等
    // cout<<par->get<string>("APP_id")<<" needs to "<<par->get<string>("function")<<endl;
    if(par->get<string>("function") == "start"){
        SSDP_Result res = SSDP_Start(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, par->get<string>("APP_name")));
        return res;
    }
    // switch(par->get<string>("function")){
    //     case "SSDP_InstantiateApp"
    // }
    // cout<<par->get<string>("source")<<endl;

    return SSDP_OK;
    
}

CmdProcess::~CmdProcess(){
    delete par;
}