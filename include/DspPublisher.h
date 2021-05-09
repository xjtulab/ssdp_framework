#include "HelloWorld.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h>  //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi
#include <time.h>
#include <sys/time.h>
#include <string>
using namespace std; 

#define STREAM_HISTORY 8
#define BUFFER_SIZE UXR_CONFIG_UDP_TRANSPORT_MTU *STREAM_HISTORY

void on_topic(uxrSession *session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer *ub, uint16_t length, void *args);

class DspPublisher
{
public:
    char *ip;
    char *port;
    uxrSession session;
    uint32_t max_topics;
    uint32_t count;
    HelloWorld topic;
    //Transport
    uxrObjectId datareader_id;
    uxrObjectId datawriter_id;
    uxrStreamId reliable_out;
    uint16_t read_data_req;
    uxrUDPTransport transport;
    uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
    uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
    char topic_xml_1[255];
    char topic_xml_2[255];
    char datawriter_xml[255];
    char datareader_xml[255];

    DspPublisher(char *ip, char *port, string topic_name, uint32_t session_key);
    bool send_cmd(char *buf);

    DspPublisher();

    ~DspPublisher()
    {
        uxr_delete_session(&session);
        uxr_close_udp_transport(&transport);
    }
};