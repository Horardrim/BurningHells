#include <stdio.h>

int main(int argc, char ** argv)
{
#ifdef ENABLE_BURNING_HELLS_TEST
    printf("It's test for burning hells.\n");
#else
    printf("Welcome BurningHells adventurer.\n");
#endif
    return 0;
}