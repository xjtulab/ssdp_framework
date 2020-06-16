#include "commandprocess.h"
#include "SSDP_PRE_DATA.h"
using std::cout;
using std::endl;


CmdProcess::CmdProcess(){
    par = new parser;
    par->add<string>("source", 's', "Command source", true);
    par->add<string>("target", 't', "Command target", true);
    par->add<string>("function", 'f',"Function to be called",true);
    par->add<SSDP_HandleID>("APP_id",'a',"APP ID", true);
    par->add<int>("comp_id",'c',"Component ID", false);
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
    
    /*
    switch(par->get<string>("function")){
        case "SSDP_InstantiateApp"
    }
    cout<<par->get<string>("source")<<endl;
    */
    return SSDP_OK;
    
}

CmdProcess::~CmdProcess(){
    delete par;
}