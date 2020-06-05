#include <stdio.h>
#include <pthread.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#include "amqp_publisher.h"
#include "amqp_config.h"

static pthread_t tid;
extern amqp_connection_state_t rabbitmq_conn;
extern void die_on_amqp_error(amqp_rpc_reply_t x, char const *context);
extern void release_dead_lock();

static void * publisher_routine(void * params)
{
    (void ) params;
    amqp_bytes_t queue_name_bytes;

    // declare exchange.
    amqp_exchange_declare(rabbitmq_conn, 1, amqp_cstring_bytes(AMQP_EXCHANGE), amqp_cstring_bytes("topic"),
			0, 0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(rabbitmq_conn), "Declaring exchange");

    // declare and bind queue
    queue_name_bytes = amqp_cstring_bytes(AMQP_QUEUE_NAME);
    amqp_table_entry_t inner_entries[1];
    amqp_table_t inner_table;
    inner_entries[0].key = amqp_cstring_bytes("x-max-priority");
    inner_entries[0].value.kind = AMQP_FIELD_KIND_I32;
    inner_entries[0].value.value.i32 = 10;
    inner_table.num_entries = 1;
    inner_table.entries = inner_entries;
    amqp_queue_declare_ok_t *r = amqp_queue_declare(rabbitmq_conn, 1, queue_name_bytes, 0, 1, 0, 0, inner_table);
    die_on_amqp_error(amqp_get_rpc_reply(rabbitmq_conn), "Declaring queue");
    amqp_bytes_t declared_queue = amqp_bytes_malloc_dup(r->queue);
    if (declared_queue.bytes == NULL) {
        fprintf(stderr, "Out of memory while copying queue name");
        return NULL;
    }
    amqp_queue_bind(rabbitmq_conn, 1, declared_queue, amqp_cstring_bytes(AMQP_EXCHANGE), 
                 amqp_cstring_bytes(AMQP_ROUTING_KEY), amqp_empty_table);

    // publish message
    amqp_basic_publish(rabbitmq_conn, 1, amqp_cstring_bytes(AMQP_EXCHANGE),
                       amqp_cstring_bytes(AMQP_ROUTING_KEY), 0, 0, NULL,
                       amqp_cstring_bytes("Message from publisher to BuringHells through river of flame."));
    amqp_rpc_reply_t amqp_publish_reply = amqp_get_rpc_reply(rabbitmq_conn);
    if (amqp_publish_reply.reply_type != AMQP_RESPONSE_NORMAL)
    {
        fprintf(stderr, "error in publishing message.%d\n", amqp_publish_reply.reply_type);
    }
    fflush(stdout);
    amqp_bytes_free(declared_queue);
    return NULL;
}

void start_publisher()
{
    pthread_create(&tid, NULL, publisher_routine, NULL);
    pthread_detach(tid);
}
