#ifndef SSDP_LOG_H
#define SSDP_LOG_H

#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging
#include "spdlog/sinks/stdout_color_sinks.h"

#include "SSDP_PRE_DATA.h"

SSDP_HandleID SSDP_GetErrorQueue(SSDP_Result result);
SSDP_Result SSDP_Log(SSDP_HandleID fromid, SSDP_HandleID targetid, SSDP_Message msg, SSDP_Buffer_Size nb);
SSDP_Result SSDP_LogInit();
#endif
