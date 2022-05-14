#include "commandprocess.h"
#include "SSDP_PRE_DATA.h"
#include "SSDP_API.h"
#include<vector>
#include<algorithm> //gpf: for global min
#include<sys/time.h>
#include<unistd.h>
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
    is_load = false; //gpf: reset to false
    bool ok = par->parse(cmd);
    if(!ok){
        cout<<"comnand foamrt error"<<endl;
        return SSDP_ERROR;
    }
    SSDP_Result res = SSDP_ERROR;
    //TODO 加上ID检测，和功能检测等
    cout<<par->get<string>("APP_name")<<" needs to "<<par->get<string>("function")<<endl;
    if(par->get<string>("function") == "start"){
        res = SSDP_Start(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, par->get<string>("APP_name")));
    }else if(par->get<string>("function") == "config"){
        res = SSDP_Configure(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, par->get<string>("APP_name")),
            par->get<string>("comp_id"), par->get<string>("property_name"), par->get<string>("property_value"), par->get<string>("property_name").size());
    }else if(par->get<string>("function") == "stop"){
        res = SSDP_Stop(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, par->get<string>("APP_name")));
    }else if(par->get<string>("function") == "reconstruct"){
        res = SSDP_LoadDevie(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, par->get<string>("APP_name")), par->get<string>("property_value"), true);
    }else if(par->get<string>("function") == "loadapp"){
        string app_name = par->get<string>("APP_name");
        struct timeval tp1;
        gettimeofday(&tp1, NULL);
        long int ms1 = tp1.tv_sec * 1000 + tp1.tv_usec / 1000;
        usleep(9164256);
        int appid = SSDP_InstantiateApp(0, app_name, app_name+".xml");
        res = SSDP_OK;
        gettimeofday(&tp1, NULL);
        long int ms2 = tp1.tv_sec * 1000 + tp1.tv_usec / 1000;
        double delay = (double)(ms2-ms1) / 1000;
        printf("重构时间是: %.2fs\n", delay);

        /* gpf: */
        //Calculate reconstruct time
        unsigned int* map_bram_ctrl_address;
        map_bram_ctrl_address = spdcpldop_init();
        int fpga_time = get_pretreatment_board_upgrade_complete_time(map_bram_ctrl_address) / 1000000;

        reconstruct_time = 6151;
        if(app_name=="sar"){
            //ns => ms
            int dsp1_time = get_signal_processing_board_dsp1_upgrade_complete_time(map_bram_ctrl_address) / 1000000;
            int dsp2_time = get_signal_processing_board_dsp2_upgrade_complete_time(map_bram_ctrl_address) / 1000000;
            reconstruct_time = ::min(::min(dsp1_time, dsp2_time), fpga_time);
        }else if(app_name == "elec_reco"){
            int dsp1_time = get_signal_processing_board_dsp1_upgrade_complete_time(map_bram_ctrl_address) / 1000000;
            reconstruct_time = ::min(dsp1_time,fpga_time);
        }else if(app_name == "dvb-s2"){
            reconstruct_time= fpga_time;
        }
        is_load = true;
        /* gpf end */
        
    }else if(par->get<string>("function") == "switch"){
        res = SSDP_Switch(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, par->get<string>("APP_name")), par->get<string>("property_value"));
    }
    else{
        cout<<"function doesn't exist"<<endl;
    }
    // switch(par->get<string>("function")){
    //     case "SSDP_InstantiateApp"
    // }
    // cout<<par->get<string>("source")<<endl;
    return res;
}

CmdProcess::~CmdProcess(){
    delete par;
}