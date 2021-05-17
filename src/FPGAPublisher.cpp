#include "microdds/FPGAPublisher.h"

static void on_topic(uxrSession *session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer *ub, uint16_t length, void *args)
{
    (void)session;
    (void)object_id;
    (void)request_id;
    (void)stream_id;
    (void)length;
    HelloWorld topic;
    HelloWorld_deserialize_topic(ub, &topic);
    printf("Received topic: %s, id: %i\n", topic.message, topic.index);
    uint32_t *count_ptr = (uint32_t *)args;
    (*count_ptr)++;
    if (topic.message[strlen(topic.message) - 1] == '\n')
        topic.message[strlen(topic.message) - 1] = '\0';
    printf("recv_data : %s\n", topic.message);
}

FPGAPublisher::FPGAPublisher(){
    printf("create FPGA dds client\n");
}

FPGAPublisher::FPGAPublisher(char *ip, char *port, string topic_name, uint32_t session_key)
{
    this->ip = ip;
    this->port = port;
    this->first = true;
    this->count = 0;
    // Transport
    //uxrUDPPlatform udp_platform;
    //uxrTCPTransport transport;
    //uxrTCPPlatform tcp_platform;
    //if(!uxr_init_tcp_transport(&transport, &tcp_platform, UXR_IPv4, ip, port))
    if (!uxr_init_udp_transport(&this->transport, &udp_platform,UXR_IPv4, ip, port))
    {
        printf("Error at create transport.\n");
        return;
    }

    // Session
    uxr_init_session(&this->session, &this->transport.comm, session_key);
    uxr_set_topic_callback(&this->session, on_topic, &this->count);
    if (!uxr_create_session(&this->session))
    {
        printf("Error at create session.\n");
        return;
    }

    // Streams
    //uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
    //printf("buf size : %d \n", BUFFER_SIZE);
    this->reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    //uxrStreamId reliable_out = uxr_create_output_best_effort_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE);
    //uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
    reliable_in = uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    //uxr_create_input_best_effort_stream(&session);
    // Create entities
    participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char *participant_xml = "<dds>"
                                  "<participant>"
                                  "<rtps>"
                                  "<name>default_xrce_participant</name>"
                                  "</rtps>"
                                  "</participant>"
                                  "</dds>";
    participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);

    // topic1
    topic_id_1 = uxr_object_id(0x01, UXR_TOPIC_ID);

    const char *topic_xml_1_1 = "<dds>"
                                "<topic>"
                                "<name>%s1</name>"
                                "<dataType>HelloWorld</dataType>"
                                "</topic>"
                                "</dds>";

    sprintf(topic_xml_1, topic_xml_1_1, topic_name.data());

    topic_req_1 = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id_1, participant_id, topic_xml_1, UXR_REPLACE);
    // topic2
    uxrObjectId topic_id_2 = uxr_object_id(0x02, UXR_TOPIC_ID);
    const char *topic_xml_2_1 = "<dds>"
                                "<topic>"
                                "<name>%s2</name>"
                                "<dataType>HelloWorld</dataType>"
                                "</topic>"
                                "</dds>";
    sprintf(topic_xml_2, topic_xml_2_1, topic_name.data());
    uint16_t topic_req_2 = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id_2, participant_id, topic_xml_2, UXR_REPLACE);

    // pub
    publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    
    publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id, publisher_xml, UXR_REPLACE);
    // sub
    subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
    subscriber_req = uxr_buffer_create_subscriber_xml(&session, reliable_out, subscriber_id, participant_id, subscriber_xml, UXR_REPLACE);

    // datawriter
    datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    const char *datawriter_xml_1 = "<dds>"
                                   "<data_writer>"
                                   "<topic>"
                                   "<kind>NO_KEY</kind>"
                                   "<name>%s1</name>"
                                   "<dataType>HelloWorld</dataType>"
                                   "</topic>"
                                   "</data_writer>"
                                   "</dds>";
    sprintf(datawriter_xml, datawriter_xml_1, topic_name.data());
    // printf(datawriter_xml, datawriter_xml_1, topic_name.data());
    printf(datawriter_xml);
    datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id, datawriter_xml, UXR_REPLACE);
    // datareader
    datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
    const char *datareader_xml_1 = "<dds>"
                                   "<data_reader>"
                                   "<topic>"
                                   "<kind>NO_KEY</kind>"
                                   "<name>%s2</name>"
                                   "<dataType>HelloWorld</dataType>"
                                   "</topic>"
                                   "</data_reader>"
                                   "</dds>";

    sprintf(datareader_xml, datareader_xml_1, topic_name.data());

    datareader_req = uxr_buffer_create_datareader_xml(&session, reliable_out, datareader_id, subscriber_id, datareader_xml, UXR_REPLACE);
    // Send create entities message and wait its status
    requests[0] = participant_req;
    requests[1] = topic_req_1;
    requests[2] = subscriber_req;
    requests[3] = datareader_req;
    requests[4] = publisher_req;
    requests[5] = datawriter_req;
    if (!uxr_run_session_until_all_status(&session, 1000, requests, status, 6))
    {
        printf("Error at create entities: participant: %i topic: %i  publisher: %i darawriter: %i\n", status[0], status[1], status[2], status[3]);
        return;
    }
    else
    {
        printf("Success at create entities: participant: %i topic: %i publisher: %i darawriter: %i\n", status[0], status[1], status[2], status[3]);
    }
    delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
    this->read_data_req = uxr_buffer_request_data(&session, reliable_out, datareader_id, reliable_in, &delivery_control);
}

bool FPGAPublisher::send_cmd(const char *buf)
{
    strcpy(topic.message, buf);
    ucdrBuffer ub;
    uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
    HelloWorld_serialize_topic(&ub, &topic);

    printf("Send topic: %s, id: %i\n", topic.message, topic.index);
    uxr_run_session_time(&session,1000);
    printf("data send\n");
    uint8_t read_data_status;
    int wait_time = this->first? 3000: -1;
    if(this->first){
        this->first = false;
    }
    bool connected = uxr_run_session_until_all_status(&session, wait_time, &read_data_req, &read_data_status, 1);
    return connected;
}
