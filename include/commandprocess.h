#ifndef COMMAND_PROCESS_H
#define COMMAND_PROCESS_H
#include "cmdline.h"
#include "SSDP_API.h"
#include "SSDP_LOG.h"
using namespace cmdline;
using std::string;

class CmdProcess{
private:
    cmdline::parser* par;
public:
    CmdProcess();
    SSDP_Result ReceiveCommand(const string& cmd);
    ~CmdProcess();

    int reconstruct_time; //gpf: This is reconstruct time for app, ms
    int is_load;//gpf: whether is load 
};




#endif