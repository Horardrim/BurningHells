#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "amqp_connection.h"
#include "amqp_subscriber.h"
#include "amqp_publisher.h"

static pthread_mutex_t dead_lock;

void release_dead_lock()
{
    pthread_mutex_unlock(&dead_lock);
}

int main(int argc, char ** argv)
{
#ifdef ENABLE_BURNING_HELLS_TEST
    pthread_mutex_init(&dead_lock, NULL);
    connect2rabbitMQ();
    start_publisher();
    sleep(1);
    start_subscriber();

    pthread_mutex_lock(&dead_lock);
    pthread_mutex_lock(&dead_lock);
    printf("dead lock released.\n");
    destroy_connection();
#else

#endif
    return 0;
}
