#include "microdds/DspPublisherTwo.h"
#include <iostream>
char recv_info_buf[255] = {'0'};

void on_topic(uxrSession *session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer *ub, uint16_t length, void *args)
{
    (void)session;
    (void)object_id;
    (void)request_id;
    (void)stream_id;
    (void)length;
    HelloWorld topic;
    HelloWorld_deserialize_topic(ub, &topic);
    /* uint32_t *count_ptr = (uint32_t *)args;
    (*count_ptr)++; */
    if (topic.message[strlen(topic.message) - 1] == '\n')
        topic.message[strlen(topic.message) - 1] = '\0';
    printf("recv_data : %s, id = %d \n", topic.message, topic.index);
    strcpy(recv_info_buf, topic.message);
}

DspPublisherTwo::DspPublisherTwo(string ip, string port, string topic_name, uint32_t session_key_1, uint32_t session_key_2)
{
    this->ip = ip;
    this->port = port;
    this->count1 = 0;
    this->count2 = 0;
    this->topic_name = topic_name;

    init_dps_publisher(session_key_1, session_key_2);
    establish_connection();
}

bool DspPublisherTwo::init_dps_publisher(uint32_t session_key_1, uint32_t session_key_2)
{
    // ------ SESSION 1 ------
    // Transport

    if (!uxr_init_udp_transport(&transport_1, &udp_platform_1, UXR_IPv4, ip.c_str(), port.c_str()))
    {
        printf("Error at create transport.\n");
        return 1;
    }

    // Session

    uxr_init_session(&session_1, &transport_1.comm, session_key_1);
    if (!uxr_create_session(&session_1))
    {
        printf("Error at create session 1.\n");
        return 1;
    }

    // Streams

    reliable_out_1 = uxr_create_output_reliable_stream(&session_1, output_reliable_stream_buffer_1,
                                                       BUFFER_SIZE, STREAM_HISTORY);

    reliable_in_1 = uxr_create_input_reliable_stream(&session_1, input_reliable_stream_buffer_1,
                                                     BUFFER_SIZE, STREAM_HISTORY);

    // Create entities
    uxrObjectId participant_id_1 = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char *participant_xml_1 = "<dds>"
                                    "<participant>"
                                    "<rtps>"
                                    "<name>default_xrce_participant_1</name>"
                                    "</rtps>"
                                    "</participant>"
                                    "</dds>";
    uint16_t participant_req_1 = uxr_buffer_create_participant_xml(&session_1, reliable_out_1, participant_id_1, 0,
                                                                   participant_xml_1, UXR_REPLACE);

    uxrObjectId topic_id_1 = uxr_object_id(0x01, UXR_TOPIC_ID);
    string topic_xml_1 = "<dds>"
                         "<topic>"
                         "<name>" +
                         topic_name + "_1</name>"
                                      "<dataType>HelloWorld</dataType>"
                                      "</topic>"
                                      "</dds>";
    uint16_t topic_req_1 = uxr_buffer_create_topic_xml(&session_1, reliable_out_1, topic_id_1, participant_id_1,
                                                       topic_xml_1.c_str(), UXR_REPLACE);

    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char *publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session_1, reliable_out_1, publisher_id,
                                                             participant_id_1, publisher_xml, UXR_REPLACE);

    datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    string datawriter_xml = "<dds>"
                            "<data_writer>"
                            "<topic>"
                            "<kind>NO_KEY</kind>"
                            "<name>" +
                            topic_name + "_1</name>"
                                         "<dataType>HelloWorld</dataType>"
                                         "</topic>"
                                         "</data_writer>"
                                         "</dds>";
    datawriter_req = uxr_buffer_create_datawriter_xml(&session_1, reliable_out_1, datawriter_id,
                                                      publisher_id, datawriter_xml.c_str(), UXR_REPLACE);

    // Send create entities message and wait its status
    uint16_t requests_1[4] = {
        participant_req_1, topic_req_1, publisher_req, datawriter_req};
    uint8_t status_1[4];

    if (!uxr_run_session_until_all_status(&session_1, 1000, requests_1, status_1, 4))
    {
        printf("Error at create entities session 1\n");
        return 1;
    }

    // ------ SESSION 2 ------
    // Transport

    if (!uxr_init_udp_transport(&transport_2, &udp_platform_2, UXR_IPv4, ip.c_str(), port.c_str()))
    {
        printf("Error at create transport.\n");
        return 1;
    }

    // Session

    uxr_init_session(&session_2, &transport_2.comm, session_key_2);
    uxr_set_topic_callback(&session_2, on_topic, NULL);
    if (!uxr_create_session(&session_2))
    {
        printf("Error at create session 2.\n");
        return 1;
    }

    // Streams
    reliable_out_2 = uxr_create_output_reliable_stream(&session_2, output_reliable_stream_buffer_2,
                                                       BUFFER_SIZE, STREAM_HISTORY);

    reliable_in_2 = uxr_create_input_reliable_stream(&session_2, input_reliable_stream_buffer_2,
                                                     BUFFER_SIZE, STREAM_HISTORY);

    // Create entities
    uxrObjectId participant_id_2 = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char *participant_xml_2 = "<dds>"
                                    "<participant>"
                                    "<rtps>"
                                    "<name>default_xrce_participant_2</name>"
                                    "</rtps>"
                                    "</participant>"
                                    "</dds>";
    uint16_t participant_req_2 = uxr_buffer_create_participant_xml(&session_2, reliable_out_2, participant_id_2, 0,
                                                                   participant_xml_2, UXR_REPLACE);

    uxrObjectId topic_id_2 = uxr_object_id(0x01, UXR_TOPIC_ID);
    string topic_xml_2 = "<dds>"
                         "<topic>"
                         "<name>" +
                         topic_name + "_2</name>"
                                      "<dataType>HelloWorld</dataType>"
                                      "</topic>"
                                      "</dds>";
    uint16_t topic_req_2 = uxr_buffer_create_topic_xml(&session_2, reliable_out_2, topic_id_2, participant_id_2,
                                                       topic_xml_2.c_str(), UXR_REPLACE);

    uxrObjectId subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
    const char *subscriber_xml = "";
    uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&session_2, reliable_out_2, subscriber_id,
                                                               participant_id_2, subscriber_xml, UXR_REPLACE);

    datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
    string datareader_xml = "<dds>"
                            "<data_reader>"
                            "<topic>"
                            "<kind>NO_KEY</kind>"
                            "<name>" +
                            topic_name + "_2</name>"
                                         "<dataType>HelloWorld</dataType>"
                                         "</topic>"
                                         "</data_reader>"
                                         "</dds>";
    datareader_req = uxr_buffer_create_datareader_xml(&session_2, reliable_out_2, datareader_id,
                                                      subscriber_id, datareader_xml.c_str(), UXR_REPLACE);

    // Send create entities message and wait its status
    uint16_t requests_2[] = {
        participant_req_2, topic_req_2, subscriber_req,
        datareader_req};
    uint8_t status_2[4];

    if (!uxr_run_session_until_all_status(&session_2, 1000, requests_2, status_2, 4))
    {
        printf("Error at create entities session 2\n");
        return 1;
    }

    // Request topics of both sessions
    uxrDeliveryControl delivery_control = {
        0};
    delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
    read_data_req_2 = uxr_buffer_request_data(&session_2, reliable_out_2, datareader_id, reliable_in_2,
                                              &delivery_control);
}

bool DspPublisherTwo::send_info(char *buf, bool recv)
{
    // Session 1 publication
    HelloWorld topic;
    topic.index = ++count1;
    strcpy(topic.message, buf);
    ucdrBuffer ub;
    uint32_t topic_size_1 = HelloWorld_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session_1, reliable_out_1, datawriter_id, &ub, topic_size_1);
    HelloWorld_serialize_topic(&ub, &topic);
    printf("send info: %s, id = %d\n", topic.message, topic.index);
    bool connected = uxr_run_session_time(&session_1, 1000);
    printf("connected1: %d\n", connected);
    // Session 2 subscribe
    string cmp = buf;
    if (cmp.substr(0,4) == "reco")
    {
        std::cout<<"reconstructing"<<std::endl;
        connected &= establish_connection();
    }else if(recv){
        std::cout<<"regular message"<<std::endl;
        connected &= recv_info();
    }
    return connected;
}

bool DspPublisherTwo::recv_info()
{
    uint8_t read_data_status;
    bool connected = uxr_run_session_until_all_status(&session_2, -1, &read_data_req_2, &read_data_status, 1);
    printf("connected2: %d\n", connected);
    return connected;
}

bool DspPublisherTwo::establish_connection()
{
    uint8_t read_data_status;
    bool connected = uxr_run_session_until_all_status(&session_2, -1, &read_data_req_2, &read_data_status, 1);
    string recv_tmp = recv_info_buf;
    std::cout<<"connecting"<<std::endl;
    if (connected && !strcmp(recv_tmp.substr(0,3).c_str(), "dsp"))
    {
        printf("%s\n", recv_info_buf);
        send_info("connected ok", false);
        return true;
    }
    return false;
}

// int main(
//     int args,
//     char **argv)
// {
//     // CLI
//     if (3 > args || 0 == atoi(argv[2]))
//     {
//         printf("usage: program [-h | --help] | ip port\n");
//         return 0;
//     }

//     char *ip = argv[1];
//     char *port = argv[2];
//     uint32_t max_topics = (args == 4) ? (uint32_t)atoi(argv[3]) : 1000;

//     DspPublisherTwo dsp(ip, port, "HelloWorldTopic", 0x11111111, 0x22222222);
//     dsp.establish_connection();
//     dsp.send_info("init", true);
//     dsp.send_info("start", true);
//     dsp.send_info("config", true);
//     dsp.send_info("reconstruct", false);
    
// }