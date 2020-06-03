#include <stdio.h>

#include "amqp_connection.h"

int main(int argc, char ** argv)
{
#ifdef ENABLE_BURNING_HELLS_TEST
    connect2rabbitMQ();
    destroy_connection();
#else
    
#endif
    return 0;
}