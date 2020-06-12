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
    string log_str = std::to_string(fromid)+" "+std::to_string(targetid)+" "+msg;
    logger->info(log_str);
    std::cout<<fsin.use_count()<<std::endl;

}

SSDP_Result SSDP_LogInit(){
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    //TODO 文件命名方式待确定
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("tmp/logfile2", 1024*1024, 5, false);
    fsin = file_sink;
    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
    logger = new spdlog::logger("multi_sink", sink_list.begin(), sink_list.end());
    return SSDP_OK;
}