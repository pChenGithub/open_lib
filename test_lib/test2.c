#include <stdio.h>
#include "level2.h"

int main(int argc, char const *argv[])
{
    struct print_data data = {1, 2, 6};
    print_my_data(&data);
    return 0;
}


