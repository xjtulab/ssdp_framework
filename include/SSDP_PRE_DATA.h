#ifndef SSDP_PRE_DATA_H
#define SSDP_PRE_DATA_H
#include<string>
using std::string;


//预定义数据
typedef int SSDP_Access;
typedef int SSDP_Buffer_Size;
typedef int SSDP_Clock_Kind;
typedef int SSDP_File_Size;
typedef int SSDP_HandleID;
//TODO 怎么定义中断函数?
typedef int (*SSDP_ISR_Function) (...);
typedef string SSDP_Message;
typedef int SSDP_Nanoseconds;
typedef string SSDP_Property_Name;
typedef string SSDP_Property_Value;
typedef int SSDP_Queue_Max_Messages;
typedef int SSDP_Result;
typedef int SSDP_Seconds;
typedef int SSDP_TestID;
struct SSDP_TimeWarp{
    SSDP_Seconds sec;
    SSDP_Nanoseconds nassec;
    SSDP_TimeWarp(SSDP_Seconds s, SSDP_Nanoseconds n){
        sec = s;
        nassec = n;
    }
};
typedef int SSDP_TimeRate;
typedef int SSDP_Type;


//TODO 确定常量的值
const SSDP_Access SSDP_ACCESS_APPEND = 1;
const SSDP_Access SSDP_ACCESS_BOTH = 2;
const SSDP_Access SSDP_ACCESS_READ = 3;
const SSDP_Access SSDP_ACCESS_WRITE =4;
const SSDP_Result SSDP_OK =0;
const SSDP_Result SSDP_ERROR = -1;
const SSDP_HandleID SSDP_ERROR_QUEUE = 1;
const SSDP_Result SSDP_FATAL = -2;
const SSDP_HandleID SSDP_FATAL_QUEUE =2;
const SSDP_HandleID SSDP_TELEMETRY_QUEUE = 3;
const SSDP_HandleID SSDP_OE_HANDLE_ID = 0;
const SSDP_Type SSDP_TYPE_BINARY = 0;
const SSDP_Type SSDP_TYPE_TEXT = 1;
const SSDP_Result SSDP_WARNING = -3;
const SSDP_HandleID SSDP_WARNING_QUEUE = 4;
const string SSDP_OE_HANDLE_NAME("SSDP_OE_V1");
const string SSDP_DEFAULT_CLOCK_NAME("default_clock");
const SSDP_Clock_Kind SSDP_DEFAULT_CLOCK_KIND = 1;
//TODO 时钟怎么确定的？——Integer accuracy of time rate in number of parts per second.
const SSDP_TimeRate SSDP_TIME_RATE_PPS = 1;

#define SSDP_MAX_PROPERTY_NAME_SIZE 128
#define SSDP_MAX_PROPERTY_VALUE_SIZE 128
#define SSDP_MAX_PATH_NAME_SIZE 128
#define SSDP_MAX_HANDLE_NAME_SIZE 128
#define SSDP_MAX_LOG_MESSAGE_SIZE 1024
#define SSDP_MAX_QUEUE_MESSAGES 1024

#endif
