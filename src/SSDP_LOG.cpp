#include "SSDP_PRE_DATA.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <string>
#include <iostream>
using std::string;
spdlog::logger* logger;
std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> fsin;

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
    //TODO 日志格式还要修改
    //TODO 添加对fatal 错误的操作
    string targetid_str = std::to_string(targetid);
    if (targetid == SSDP_WARNING_QUEUE){
        targetid_str = "WARNING";
    }else if(targetid == SSDP_ERROR_QUEUE){
        targetid_str = "ERROR";
    }else if(targetid == SSDP_FATAL_QUEUE){
        targetid_str = "FATAL";
    }
    string log_str = std::to_string(fromid)+" "+targetid_str+" "+msg;
    logger->info(log_str);
    // std::cout<<fsin.use_count()<<std::endl;

}

SSDP_Result SSDP_LogInit(){
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    //TODO 文件命名方式待确定
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("tmp/logfile", 1024*1024, 5, false);
    fsin = file_sink;
    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
    logger = new spdlog::logger("multi_sink", sink_list.begin(), sink_list.end());
    string log_pattern = "[%D %T] %v";
    logger->set_pattern(log_pattern);
    return SSDP_OK;
}