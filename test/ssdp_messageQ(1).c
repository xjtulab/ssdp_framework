/*
 * ssdp_messageQ.c
 *
 *  Created on: 2021Äê5ÔÂ7ÈÕ
 *      Author: fh
 */

#include "ssdp_messageQ.h"

bool connected = true;
uint32_t count = 0;
//session1
uxrUDPTransport transport_1;
uxrUDPPlatform udp_platform_1;
uxrSession session_1;
uint8_t output_reliable_stream_buffer_1[BUFFER_SIZE];
uxrStreamId reliable_out_1;
int8_t input_reliable_stream_buffer_1[BUFFER_SIZE];
uxrStreamId reliable_in_1;
uxrObjectId datareader_id;
uint16_t datareader_req;
uint16_t read_data_req_1;

//session2
uxrUDPTransport transport_2;
uxrUDPPlatform udp_platform_2;
uxrSession session_2;
uint8_t output_reliable_stream_buffer_2[BUFFER_SIZE];
uxrStreamId reliable_out_2;
uint8_t input_reliable_stream_buffer_2[BUFFER_SIZE];
uxrStreamId reliable_in_2;
uxrObjectId datawriter_id;
uint16_t datawriter_req;

HelloWorld topic;


void on_topic(
    uxrSession *session,
    uxrObjectId object_id,
    uint16_t request_id,
    uxrStreamId stream_id,
    struct ucdrBuffer *ub,
    uint16_t length,
    void *args)
{
    (void)session;
    (void)object_id;
    (void)request_id;
    (void)stream_id;
    (void)length;
    //        recv_new_cmd = true;
    HelloWorld_deserialize_topic(ub, &topic);
    if (topic.message[strlen(topic.message) - 1] == '\n')
        topic.message[strlen(topic.message) - 1] = '\0';
    printf("Received topic: %s, id: %i\n", topic.message, topic.index);
    uint32_t *count_ptr = (uint32_t *)args;
    (*count_ptr)++;
    //        Semaphore_post(sem1);
}

bool ssdp_init_udp(char *ip, char *port, char *topic_reader, char* topic_writer, uint32_t reader_session_key, uint32_t writer_session_key)
{
    // ------ SESSION 1 ------
       // Transport
       if (!uxr_init_udp_transport(&transport_1, &udp_platform_1,UXR_IPv4, ip, port))
       {
           printf("Error at create transport.\n");
           return 1;
       }

       // Session

       uxr_init_session(&session_1, &transport_1.comm, reader_session_key);
       uxr_set_topic_callback(&session_1, on_topic, NULL);
       if (!uxr_create_session(&session_1))
       {
           printf("Error at create session 1.\n");
           return 0;
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
       const char *topic_xml_1_1 = "<dds>"
                                 "<topic>"
                                 "<name>%s</name>"
                                 "<dataType>HelloWorld</dataType>"
                                 "</topic>"
                                 "</dds>";
       char topic_xml_1[255];
       sprintf(topic_xml_1,topic_xml_1_1,topic_reader);
       uint16_t topic_req_1 = uxr_buffer_create_topic_xml(&session_1, reliable_out_1, topic_id_1, participant_id_1,
                                                          topic_xml_1, UXR_REPLACE);

       uxrObjectId subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
       const char *subscriber_xml = "";
       uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&session_1, reliable_out_1, subscriber_id,
                                                                  participant_id_1, subscriber_xml, UXR_REPLACE);

       datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
       const char *datareader_xml_1 = "<dds>"
                                    "<data_reader>"
                                    "<topic>"
                                    "<kind>NO_KEY</kind>"
                                    "<name>%s</name>"
                                    "<dataType>HelloWorld</dataType>"
                                    "</topic>"
                                    "</data_reader>"
                                    "</dds>";
       char datareader_xml[255];
       sprintf(datareader_xml,datareader_xml_1,topic_reader);
       datareader_req = uxr_buffer_create_datareader_xml(&session_1, reliable_out_1, datareader_id,
                                                                  subscriber_id, datareader_xml, UXR_REPLACE);

       // Send create entities message and wait its status
       uint16_t requests_1[4] = {
           participant_req_1, topic_req_1, subscriber_req, datareader_req};
       uint8_t status_1[4];

       if (!uxr_run_session_until_all_status(&session_1, 3000, requests_1, status_1, 4))
       {
           printf("Error at create entities session 1\n");
           return 0;
       }

       // Request topics of both sessions
       uxrDeliveryControl delivery_control = {
           0};
       delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
       read_data_req_1 = uxr_buffer_request_data(&session_1, reliable_out_1, datareader_id, reliable_in_1,
                                                          &delivery_control);

       // ------ SESSION 2 ------ publisher
       // Transport

       if (!uxr_init_udp_transport(&transport_2, &udp_platform_2, UXR_IPv4, ip, port))
       {
           printf("Error at create transport.\n");
           return 1;
       }

       // Session

       uxr_init_session(&session_2, &transport_2.comm, writer_session_key);

       if (!uxr_create_session(&session_2))
       {
           printf("Error at create session 2.\n");
           return 1;
       }

       // Streams
       reliable_out_2 = uxr_create_output_reliable_stream(&session_2, output_reliable_stream_buffer_2,
                                                                      BUFFER_SIZE, STREAM_HISTORY);

       uint8_t input_reliable_stream_buffer_2[BUFFER_SIZE];
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
       const char *topic_xml_2_1 = "<dds>"
                                 "<topic>"
                                 "<name>%s</name>"
                                 "<dataType>HelloWorld</dataType>"
                                 "</topic>"
                                 "</dds>";
       char topic_xml_2[255];
       sprintf(topic_xml_2,topic_xml_2_1,topic_writer);
       uint16_t topic_req_2 = uxr_buffer_create_topic_xml(&session_2, reliable_out_2, topic_id_2, participant_id_2,
                                                          topic_xml_2, UXR_REPLACE);

       uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
       const char *publisher_xml = "";
       uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session_2, reliable_out_2, publisher_id,
                                                                participant_id_2, publisher_xml, UXR_REPLACE);

       datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
       const char *datawriter_xml_1 = "<dds>"
                                    "<data_writer>"
                                    "<topic>"
                                    "<kind>NO_KEY</kind>"
                                    "<name>%s</name>"
                                    "<dataType>HelloWorld</dataType>"
                                    "</topic>"
                                    "</data_writer>"
                                    "</dds>";
       char datawriter_xml[255];
       sprintf(datawriter_xml,datawriter_xml_1,topic_writer);
       datawriter_req = uxr_buffer_create_datawriter_xml(&session_2, reliable_out_2, datawriter_id,
                                                                  publisher_id, datawriter_xml, UXR_REPLACE);
       // Send create entities message and wait its status
       uint16_t requests_2[] = {
           participant_req_2, topic_req_2, publisher_req,
           datawriter_req};
       uint8_t status_2[4];

       if (!uxr_run_session_until_all_status(&session_2, 3000, requests_2, status_2, 4))
       {
           printf("Error at create entities session 2\n");
           return 1;
       }
}



bool ssdp_pub_topic(char *topic)
{
    return true;
}

bool ssdp_sub_topic(char *topic)
{
    return true;
}

int ssdp_udp_send_data(char *reVal)
{
    HelloWorld topic2;
    topic2.index = ++count;
    strcpy(topic2.message, reVal);
    ucdrBuffer ub;
    uint32_t topic_size = HelloWorld_size_of_topic(&topic2, 0);
    uxr_prepare_output_stream(&session_2, reliable_out_2, datawriter_id, &ub, topic_size);
    HelloWorld_serialize_topic(&ub, &topic2);
    connected = uxr_run_session_time(&session_2, 60);
    //printf("send status %d   send message %s  \n", connected, topic2.message);
    return connected;
}



int ssdp_udp_recv_data(bool start_t, char *buf, int len)
{
    //printf("%d \n", count);
    uint8_t read_data_status;
    if (start_t)
    {
        printf("no bolck \n");
        connected = uxr_run_session_until_all_status(&session_1, 100, &read_data_req_1, &read_data_status, 1);
    }
    else
    {
        //printf("block \n");
        connected = uxr_run_session_until_all_status(&session_1, 2000, &read_data_req_1, &read_data_status, 1);
    }
    strcpy(buf, topic.message);
    //printf("%s\n",topic.message);
    if (!connected)
        //Semaphore_post(sem1);
        return 0;
    return 1;
}

