#include "level2.h"
#include <stdio.h>

int print_my_data(struct print_data* data) {
    if (NULL==data)
        return -1;
    printf("print_data a %d\n", data->a);
    //printf("print_data b %d\n", data->b);
    printf("print_data c %d\n", data->c);
    printf("print_data d %d\n", data->d);
    return 0;
}

