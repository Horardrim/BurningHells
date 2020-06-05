#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#include "amqp_subscriber.h"
#include "amqp_config.h"

extern void release_dead_lock();

static pthread_t tid;
extern amqp_connection_state_t rabbitmq_conn;

static void dump_amqp_message()
{
    size_t body_remaining;
    amqp_frame_t frame;

    int res = amqp_simple_wait_frame(rabbitmq_conn, &frame);
    if (res < 0 || frame.frame_type != AMQP_FRAME_HEADER)
    {
        fprintf(stderr, "Wrong frame: %d and Wrong return: %d in read AMQP_FRAME_HEADER\n", frame.frame_type, res);
        return;
    }

    body_remaining = frame.payload.properties.body_size;
    printf("frame size: %d, frame type: %d, body_remaining: %ld\n", res, frame.frame_type, body_remaining);
    while (body_remaining)
    {
        res = amqp_simple_wait_frame(rabbitmq_conn, &frame);
        if (frame.frame_type != AMQP_FRAME_BODY)
        {
            fprintf(stderr, "Wrong frame: %d and Wrong return: %d in read AMQP_FRAME_BODY\n", frame.frame_type, res);
            return;
        }

        body_remaining -= frame.payload.body_fragment.len;
    }
    printf("amqp message: %s\n", (char *)frame.payload.body_fragment.bytes);
}

static void * subscriber_routine(void * params)
{
    (void ) params;
    amqp_rpc_reply_t get_reply = amqp_basic_get(rabbitmq_conn, 1, amqp_cstring_bytes(AMQP_QUEUE_NAME), 1);
    printf("reply_type: %d, reply_id: %d\n", get_reply.reply_type, get_reply.reply.id == AMQP_BASIC_GET_OK_METHOD);
    dump_amqp_message();
    sleep(1);
    release_dead_lock();
    return NULL;
}

void start_subscriber()
{
    pthread_create(&tid, NULL, subscriber_routine, NULL);
    pthread_detach(tid);
}
