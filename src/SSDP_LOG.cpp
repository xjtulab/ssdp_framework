#include "SSDP_PRE_DATA.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <string>
#include <iostream>
using std::string;
spdlog::logger* logger;

SSDP_HandleID SSDP_GetErrorQueue(SSDP_Result result){
    SSDP_HandleID res = SSDP_ERROR_QUEUE;
    if(result == SSDP_FATAL){
        res = SSDP_FATAL_QUEUE;
    }else if(result == SSDP_ERROR){
        res = SSDP_ERROR_QUEUE;
    }else if(result = SSDP_WARNING){
        res = SSDP_WARNING_QUEUE;
    }
    return res;
}

SSDP_Result SSDP_Log(SSDP_HandleID fromid, SSDP_HandleID targetid, SSDP_Message msg, SSDP_Buffer_Size nb){
    string log_str = std::to_string(fromid)+" "+std::to_string(targetid)+" "+msg;
    logger->info(log_str);

}

SSDP_Result SSDP_LogInit(){
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("tmp/logfile2", 1024*1024, 5, false);
    std::cout<< file_sink.use_count()<<std::endl;
    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
    std::cout<< file_sink.use_count()<<std::endl;
    logger = new spdlog::logger("multi_sink", sink_list.begin(), sink_list.end());
    std::cout<< file_sink.use_count()<<std::endl;
    return SSDP_OK;
}