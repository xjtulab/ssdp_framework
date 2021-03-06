#include "SSDP_API.h"
#include "SSDP.h"
#include "SSDP_PRE_DATA.h"
#include <string>
#include "base_app.h"
#include "base_device.h"
#include <dlfcn.h>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <unistd.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "devices/DeviceDSP.h"
#include "devices/DeviceFPGA.h"
#include "devices/DeviceZED.h"
// #define ARM_BUILD
#ifdef ARM_BUILD
    extern "C"{
        #include "58/libmc3s029zesensorinfoget.h"
        #include "58/libmc3s028zecpldcfg.h"
    }
#endif
using std::string;
using std::map;
using std::cout;
using std::endl;
using std::set;

//变量区

//apptable存放启动的应用对象，key是handleid（一个数字），value是（base_APP的指针)
map<SSDP_HandleID, std::shared_ptr<AppBase>> apptable;
//devicetable存放设备对象
map<SSDP_HandleID, std::shared_ptr<DeviceBase>> devicetable;
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
            return res;
        }
        ++iter;
    }
    auto dev_iter = devicetable.begin();
    while(dev_iter != devicetable.end()){
        if(dev_iter->second->DEV_GetHandleName() == targetname ){
            res = dev_iter->second->DEV_GetHandleID();
            return res;
        }
        ++dev_iter;
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
    }else if(devicetable.count(toid) > 0){
        res_str = devicetable[toid]->DEV_GetHandleName();
        res = SSDP_OK;
    }
    targetname = res_str;
    return res;
}

bool SSDP_IsOK(SSDP_Result result){
    if (result >=0) return true;
    else return false;
}

static void DSP_Reset(){
    auto dev_iter = devicetable.begin();
    while(dev_iter != devicetable.end()){
        if(dev_iter->second->DEV_GetHandleName().substr(0, 3) == "dsp" ){
            SSDP_LoadDevie(SSDP_OE_HANDLE_ID, dev_iter->second->DEV_GetHandleID(), "0", true);
        }
        ++dev_iter;
    }
}

static inline void Reset1848(){
    #ifdef ARM_BUILD
        system("/bin/cps1848reset");
        usleep(200000);
        system("/bin/setcps1848");
    #endif 
}

//创建应用实例
SSDP_HandleID SSDP_InstantiateApp(SSDP_HandleID fromid, string handlename, string filepath ){
    //TODO 主控板1848 复位
    bool isNewCode = true;
    bool isSar = (handlename == "sar");//gpf: set sar flag

    if(isNewCode){
        Reset1848();
    }
    // DSP_Reset();
    set<string> used_device;
    //TODO 有存在同名app时先检查删除掉
    cout<<"app nums: "<<apptable.size()<<endl;
    //TODO 解析配置文件，读取comp列表，并添加comp
    rapidxml::file<> fdoc(filepath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());
    rapidxml::xml_node<> *waveform = doc.first_node();
    auto new_app = std::make_shared<AppBase>(handlename, SSDP_GetNewHandleID(),&app_functable);
    //cout<<new_app.use_count()<<endl;
    apptable.insert(std::make_pair(new_app->APP_GetHandleID(),new_app));
    rapidxml::xml_node<> *comp = waveform->first_node("components")->first_node("component");
    while(comp){
        // cout<<comp->first_node("objId")->value()<<endl;
        string dev_name = comp->first_node("resourceInfo")->first_node("info")->first_node("name")->value();
        SSDP_HandleID dev_id =  SSDP_HandleRequest(fromid, comp->first_node("resourceInfo")->first_node("info")->first_node("name")->value());
        cout<<"dev_id: "<<dev_id<<endl;
        new_app->Add_Component(comp->first_node("objId")->value(), comp->first_node("resourceInfo")->first_node("info")->first_node("codeLocation")->value(), \
            dev_id, comp->first_node("componenId")->value());
        //重构设备或同步设备
        if(used_device.find(dev_name)==used_device.end()){
            SSDP_LoadDevie(SSDP_OE_HANDLE_ID, dev_id, comp->first_node("resourceInfo")->first_node("info")->first_node("codeLocation")->value(), isNewCode);
            used_device.emplace(dev_name);
        }
        //依次添加参数名/地址对
        rapidxml::xml_node<> *parameter = comp->first_node("parameters")->first_node("parameter");
        while(parameter){
            new_app->Add_Component_Parameter(comp->first_node("objId")->value(), parameter->first_node("name")->value(), parameter->first_node("address")->value());
            parameter = parameter->next_sibling();
        }

        comp = comp->next_sibling();
    }

    //重构设备
    // SSDP_Write(SSDP_OE_HANDLE_ID, SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "fpga1"), 0, "switch2_elec",14);
    

    //TODO 添加每个设备重构的部分！！！！！！！！！！！！！！！！！！！！！
    //遍历app对象的component_list找到目标设备和程序，调用SSDP_LoadDevie()接口依次上载程序，完成DDS的同步（接收dsp ready，发送connected ok，发送EMPTY TEST）
    //有几个设备就调用几次SSDP_LoadDevie函数，参数是设备的id和codeloacation。
    //cout<<new_app.use_count()<<endl;
    //TODO 进行应用属性配置，启动等
    //new_app.use_count();

    /* gpf: Sar application will call additional function */
    if(isSar){
        system("/bin/sarapplication");
        cout<<"Start xxx done."<<endl;
    }
    /* gpf end*/

    return new_app->APP_GetHandleID();
}

SSDP_Result SSDP_Start(SSDP_HandleID formid, SSDP_HandleID toid){
    if(toid == -1){
        cout<<"device not defined yet"<<endl;
        return SSDP_OK;
    }
    if (apptable.count(toid) != 0){
        if(toid == SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "dvb-s2")){
            apptable[toid]->DVB_Start();
        }else if(toid == SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "sar")){
            apptable[toid]->SAR_Start();
        }else{
            apptable[toid]->ELEC_Start();
        }
        return SSDP_OK;
    }
    else if(devicetable.count(toid) != 0){
        string appName;
        SSDP_Result res = SSDP_GetHandleName(SSDP_OE_HANDLE_ID, formid, appName);
        devicetable[toid]->DEV_Start(appName);
        return SSDP_OK;
    }else{
        return SSDP_ERROR;
    }
}

SSDP_Result SSDP_Stop(SSDP_HandleID formid, SSDP_HandleID toid){
    if(toid == -1){
        cout<<"device not defined yet"<<endl;
        return SSDP_OK;
    }
    if (apptable.count(toid) != 0){
        if(toid == SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "dvb-s2")){
            apptable[toid]->DVB_Stop();
        }else if(toid == SSDP_HandleRequest(SSDP_OE_HANDLE_ID, "sar")){
            apptable[toid]->SAR_Stop();
        }else{
            apptable[toid]->ELEC_Stop();
        }
        return SSDP_OK;
    }
    else if(devicetable.count(toid) != 0){
        string appName;
        SSDP_Result res = SSDP_GetHandleName(SSDP_OE_HANDLE_ID, formid, appName);
        devicetable[toid]->DEV_Stop(appName);
        return SSDP_OK;
    }else{
        return SSDP_ERROR;
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
    }else if(devicetable.count(toid) != 0){
        devicetable[toid]->DEV_Write(buffer);
        return SSDP_OK;
    }else{
        return SSDP_ERROR;
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

SSDP_Result SSDP_Configure(SSDP_HandleID fromid, SSDP_HandleID toid,string comp_id, SSDP_Property_Name name, SSDP_Property_Value value, SSDP_Buffer_Size value_size){
    if (apptable.count(toid) != 0){
        cout<<"config"<<endl;
        apptable[toid]->APP_Configure(comp_id,name,value,value_size);
        return SSDP_OK;
    }
    else if(devicetable.count(toid) != 0){
        devicetable[toid]->DEV_Configure(comp_id, name, value, value.size());
        return SSDP_OK;
    }else{
        return SSDP_ERROR;
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

SSDP_HandleID SSDP_InstantiateDevice(SSDP_HandleID fromid, string handlename, string config_file_path){
    //根据设备类型创建不同类型的对象
    rapidxml::file<> fdoc(config_file_path.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());
    rapidxml::xml_node<> *device = doc.first_node();
    string dev_type = device->first_node("devicetype")->value();
    DeviceBase* new_dev;
    if(dev_type == "dsp"){
        
        // auto new_dev_tmp = std::make_shared<DeviceDSP>(handlename,  SSDP_GetNewHandleID());
        new_dev = new DeviceDSP(handlename,  SSDP_GetNewHandleID());
        cout<<"dsp created"<<endl;
        rapidxml::xml_node<> *ddsconfig = device->first_node("ddsconfig");
        // dynamic_cast<DeviceDSP*>(new_dev)->DEV_SetPub(ddsconfig->first_node("ip")->value(), ddsconfig->first_node("port")->value(), ddsconfig->first_node("topicname")->value(), ddsconfig->first_node("session_key")->value()); 
        #ifdef ARM_BUILD
            new_dev->DEV_SetPub(ddsconfig->first_node("ip")->value(), ddsconfig->first_node("port")->value(), ddsconfig->first_node("topicname")->value(), ddsconfig->first_node("session_key")->value()); 
        #endif
    }else if(dev_type == "fpga"){
        cout<<"fpga created"<<endl;
        new_dev = new DeviceFPGA(handlename,  SSDP_GetNewHandleID());
        rapidxml::xml_node<> *ddsconfig = device->first_node("ddsconfig");
        #ifdef ARM_BUILD
            new_dev->DEV_SetPub(ddsconfig->first_node("ip")->value(), ddsconfig->first_node("port")->value(), ddsconfig->first_node("topicname")->value(), ddsconfig->first_node("session_key")->value()); 
        #endif
    }else if(dev_type == "zed"){
        cout<<"zed created"<<endl;
        new_dev = new DeviceZED(handlename, SSDP_GetNewHandleID());
        rapidxml::xml_node<> *net = device->first_node("net");
        new_dev->DEV_SetPub(net->first_node("ip")->value(), net->first_node("port")->value()," ", " ");
    }
    devicetable.insert(std::make_pair(new_dev->DEV_GetHandleID(), new_dev));
    return new_dev->DEV_GetHandleID();
    // auto new_dev1 = std::make_shared<DeviceBase>(handlename, SSDP_GetNewHandleID());
    // devicetable.insert(std::make_pair(new_dev1->DEV_GetHandleID(), new_dev1));
    // return new_dev->DEV_GetHandleID();
}

SSDP_Result SSDP_self_Init(){
    app_functable.loadDevice = &SSDP_LoadDevie;
    app_functable.handleRequest = &SSDP_HandleRequest;
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
    SSDP_Result res = SSDP_OK;
    #ifdef ARM_BUILD
    //主控板状态检查
        float  voltage_data = 0;
        float temperature_data = 0;
        if(!get_main_control_board_eth0_link_status()){
            // res = SSDP_ERROR;
            cout<<"eth0 link status error"<<endl;
        }else{
            cout<<"eth0 link status is ok"<<endl;
        }
        if(!get_main_control_board_eth1_link_status()){
            // res = SSDP_ERROR;
            cout<<"eth1 link status error"<<endl;
        }else{
            cout<<"eth1 link status is ok"<<endl;
        }
        if(!get_main_control_board_eth2_link_status()){
            // res = SSDP_ERROR;
            cout<<"eth2 link status error"<<endl;
        }else{
            cout<<"eth2 link status is ok"<<endl;
        }
        // if(!get_main_control_board_eth3_link_status()){
        //     res = SSDP_ERROR;
        //     cout<<"eth3 link status error"<<endl;
        // }else{
        //     cout<<"eth3 link status is ok"<<endl;
        // }
        // if(!get_main_control_board_sata_ready_status()){
        //     res = SSDP_ERROR;
        //     cout<<"sata ready status is not ok!"<<endl;
        // }else{
        //     cout<<"sata ready status is ok!"<<endl;
        // }
        if(!get_main_control_board_sata_link_status()){
            res = SSDP_ERROR;
            cout<<"sata link status is not ok"<<endl;
        }else{
            cout<<"sata link status is ok"<<endl;
        }
        if(!get_main_control_board_cdcm_status()){
            res = SSDP_ERROR;
            cout<<"cdcm status is not ok"<<endl;
        }else{
            cout<<"cdcm status is ok"<<endl;
        }
        if(!get_main_control_board_srio0_link_status()){
            res = SSDP_ERROR;
            cout<<"srio0 link status is not ok"<<endl;
        }else{
            cout<<"srio0 link status is ok"<<endl;
        }
        if(!get_main_control_board_srio1_link_status()){
            res = SSDP_ERROR;
            cout<<"srio1 link status is not ok"<<endl;
        }else{
            cout<<"srio1 link status is ok"<<endl;
        }
    //存储板状态检查
        unsigned int* map_bram_ctrl_address;
        map_bram_ctrl_address = spdcpldop_init();
        if(get_storage_board_zynq_srio_link_status(map_bram_ctrl_address)){
            cout<<"storage_board_zynq_srio_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"storage_board_zynq_srio_link_status is not ok"<<endl;
        }
        if(get_storage_board_fpga1_srio_link_status(map_bram_ctrl_address)){
            cout<<"storage_board_fpga1_srio_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"storage_board_fpga1_srio_link_status is not ok"<<endl;
        }
        if(get_storage_board_fpga2_srio_link_status(map_bram_ctrl_address)){
            cout<<"storage_board_fpga2_srio_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"storage_board_fpga2_srio_link_status is not ok"<<endl;
        }
        if(get_storage_board_zynq_ge_link_status(map_bram_ctrl_address)){
            cout<<"storage_board_zynq_ge_link_status is ok"<<endl;
        }else{
            res = SSDP_ERROR;
            cout<<"storage_board_zynq_ge_link_status is not ok"<<endl;
        }
        if(get_storage_board_zynq_core_status(map_bram_ctrl_address)){
            cout<<"storage_board_zynq_core_status is ok"<<endl;
        }else{
            res =SSDP_ERROR;
            cout<<"storage_board_zynq_core_status is not ok"<<endl;
        }
        spdcpldop_release();
    #endif
    return res;
}

// std::string SSDP_DeviceStatus(){
//     float storage_board_voltage1 = 0;
//     float storage_board_temperature1 = 0;
//     float voltage_data_tmp = 0;
// 	float temperature_data_tmp = 0;
//     #ifdef ARM_BUILD
//         //存储板状态
//         unsigned int* map_bram_ctrl_address;
//         map_bram_ctrl_address = spdcpldop_init();
//         storage_board_voltage1 = get_storage_board_voltage1(map_bram_ctrl_address);
//         storage_board_temperature1 = get_storage_board_temperature1(map_bram_ctrl_address);
//         spdcpldop_release();
//         //主控版状态
//         voltage_data_tmp = get_main_control_board_voltage1_vccaux();
//         temperature_data_tmp = get_main_control_board_temperature();
//     #endif 
//     std::string res;
//     res = "<devices>"
//               "<device>"
//                   "<Id>maincontrol</Id>"
//                   "<vol>"+to_string(voltage_data_tmp)+"</vol>"
//                   "<temp>"+to_string(temperature_data_tmp)+"</temp>"
//               "</device>"
//               "<device>"
//                   "<Id>storage</Id>"
//                   "<vol>"+to_string(storage_board_voltage1)+"</vol>"
//                   "<temp>"+to_string(storage_board_temperature1)+"</temp>"
//               "</device>";
//     for(auto pos = devicetable.begin(); pos != devicetable.end(); pos++ ){
//         res += pos->second->DEV_Status_Qeury();
//     }
//     res += "</devices>";
//     return res;
// }

std::string SSDP_DeviceStatus(){
    float storage_board_voltage1 = 3.3;
    float storage_board_temperature1 = 38.1;
    float voltage_data_tmp = 3.2;
	float temperature_data_tmp = 37.5;
    unsigned long total_memory = 0;
    unsigned long free_memory = 0;
    float memory_usage = 0.0;
    int cpu_usage = 0;
    // vector<bool> signal_processing_board_dsp1to4AndFPGA_upgrade_complete_status(5, false);
    // vector<int> signal_processing_board_dsp1to4AndFPGA_upgrade_complete_time(5, 0);
    std::cout<<"before get interface!"<<std::endl;
    #ifdef ARM_BUILD
        //存储板状态
        unsigned int* map_bram_ctrl_address;
        map_bram_ctrl_address = spdcpldop_init();
        storage_board_voltage1 = get_storage_board_voltage1(map_bram_ctrl_address);
        storage_board_temperature1 = get_storage_board_temperature1(map_bram_ctrl_address);
                
        //主控版状态
        voltage_data_tmp = get_main_control_board_voltage1_vccaux();
        temperature_data_tmp = get_main_control_board_temperature();
        total_memory = get_main_control_board_total_memory();
        free_memory = get_main_control_board_free_memory();
        memory_usage = (float)(total_memory - free_memory)/total_memory*100;
        cpu_usage = get_main_control_board_cpu_usage();
        // printf("cpu usage %u%\n", cpu_usage);
        // printf("memory usage is %0.2f%\n", memory_usage);
        //2021.6.29加入信号处理板重构状态和时间
        
        spdcpldop_release();
    #endif 
    std::string res;
    res = "<devices>"
              "<device>"
                  "<Id>maincontrol</Id>"
                  "<vol>"+to_string(voltage_data_tmp)+"</vol>"
                  "<temp>"+to_string(temperature_data_tmp)+"</temp>"
                  "<mem>"+to_string(memory_usage)+"%</mem>"
                  "<cpu>"+to_string(cpu_usage)+"%</cpu>"
              "</device>"
              "<device>"
                  "<Id>storage</Id>"
                  "<vol>"+to_string(storage_board_voltage1)+"</vol>"
                  "<temp>"+to_string(storage_board_temperature1)+"</temp>"
              "</device>";
    //调用函数得到的字符串和循环里的拼接
    for(auto pos = devicetable.begin(); pos != devicetable.end(); pos++){
        res += pos->second->DEV_Status_Qeury(); 
    }
    res += "</devices>";
    std::cout<<res<<std::endl;
       
    return res;
}


SSDP_Result SSDP_DeleteDevice(){
    devicetable.clear();
    apptable.clear();
}

SSDP_Result SSDP_LoadDevie(SSDP_HandleID fromid, SSDP_HandleID toid, string file_path, bool newCode){
    SSDP_Result res;
    res = devicetable[toid]->DEV_Load(file_path, newCode);
    return res;
}

std::string SSDP_GetDeviceList(){
    string res;
    for(auto pos = devicetable.begin(); pos != devicetable.end(); pos++){
        string tmp;
        SSDP_GetHandleName(SSDP_OE_HANDLE_ID, pos->first, tmp);
        res += tmp+";";
    }
    return res;
}

// SSDP_Result SSDP_SwitchSar(SSDP_HandleID fromid, SSDP_HandleID toid){
//     if(toid == -1){
//         cout<<"app not defined yet"<<endl;
//         return SSDP_OK;
//     }
//     if (apptable.count(toid) != 0){
//         //停止雷达在原先位置的运行
//         apptable[toid]->APP_Stop();
//         //更换设备位置,dsp重构，fpga发送switch指令
//         apptable[toid]->Sar_switch();
//         return SSDP_OK;
//     }
// }

SSDP_Result SSDP_Switch(SSDP_HandleID fromid, SSDP_HandleID toid, string target_dsp){
    if(toid == -1){
        cout<<"app not defined yet"<<endl;
        return SSDP_OK;
    }

    if (toid == SSDP_HandleRequest(fromid, "sar") && apptable.count(toid) != 0){
        apptable[toid]->SAR_Stop();
        Reset1848();
        //更换设备位置,dsp重构，fpga发送switch指令
        apptable[toid]->Sar_switch();
        return SSDP_OK;
    }else if(toid == SSDP_HandleRequest(fromid, "elec_reco") && apptable.count(toid) != 0){
        apptable[toid]->ELEC_Stop();
        usleep(2000000);
        Reset1848();
        //更换设备位置,dsp重构，fpga发送switch指令
        apptable[toid]->Elec_switch(target_dsp);
        return SSDP_OK;

    }
}