#include <stdio.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include "amqp_connection.h"

static const char * amqp_hostname = "localhost";
static const int amqp_port = 5672;
static const char * amqp_user = "guest";
static const char * amqp_pwd = "guest";
static const char * amqp_vhost = "/";
amqp_connection_state_t rabbitmq_conn;

void connect2rabbitMQ()
{
    amqp_socket_t * rabbitmq_socket;
    amqp_rpc_reply_t rabbitmq_login_reply;
    int status;
    rabbitmq_conn = amqp_new_connection();
    rabbitmq_socket = amqp_tcp_socket_new(rabbitmq_conn);
    status = amqp_socket_open(rabbitmq_socket, amqp_hostname, amqp_port);

    if (status)
    {
        perror("failed to connect rabbit mq\n");
        return;
    }

    rabbitmq_login_reply = amqp_login(rabbitmq_conn, amqp_vhost, 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, amqp_user, amqp_pwd);
    if (rabbitmq_login_reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION)
    {
        perror("amqp login error\n");
        return;
    }

    amqp_channel_open(rabbitmq_conn, 1);
    printf("connect rabbit mq successfully.\n");
}

void destroy_connection()
{
    amqp_channel_close(rabbitmq_conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(rabbitmq_conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(rabbitmq_conn);
}
