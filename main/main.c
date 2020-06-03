#include <stdio.h>

#include "amqp_connection.h"

int main(int argc, char ** argv)
{
#ifdef ENABLE_BURNING_HELLS_TEST
    printf("It's test for burning hells.\n");
#else
    connect2rabbitMQ();
    destroy_connection();
#endif
    return 0;
}