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

static void on_topic(uxrSession *session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer *ub, uint16_t length, void *args);

class DspPublisherTwo
{
private:
    string ip;
    string port;
    string topic_name;
    //session1
    uxrUDPTransport transport_1;
    uxrUDPPlatform udp_platform_1;
    uxrSession session_1;
    uint8_t output_reliable_stream_buffer_1[BUFFER_SIZE];
    uxrStreamId reliable_out_1;
    uint8_t input_reliable_stream_buffer_1[BUFFER_SIZE];
    uxrStreamId reliable_in_1;
    uxrObjectId datawriter_id;
    uint16_t datawriter_req;
    uint32_t count1;

    //session2
    uxrUDPTransport transport_2;
    uxrUDPPlatform udp_platform_2;
    uxrSession session_2;
    uint8_t output_reliable_stream_buffer_2[BUFFER_SIZE];
    uxrStreamId reliable_out_2;
    uint8_t input_reliable_stream_buffer_2[BUFFER_SIZE];
    uxrStreamId reliable_in_2;
    uxrObjectId datareader_id;
    uint16_t datareader_req;
    uint16_t read_data_req_2;
    uint32_t count2;

public:
    DspPublisherTwo(string ip, string port, string topic_name, uint32_t session_key_1, uint32_t session_key_2);
    bool init_dps_publisher(uint32_t session_key_1, uint32_t session_key_2);
    bool send_info(char *buf,bool recv);
    bool recv_info();

    ~DspPublisherTwo()
    {
        uxr_delete_session(&session_1);
        uxr_close_udp_transport(&transport_1);
        uxr_delete_session(&session_1);
        uxr_close_udp_transport(&transport_1);
    }
};