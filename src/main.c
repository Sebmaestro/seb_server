#include <stdio.h>
#include "../include/net.h"

void start_server() {
    printf("Starting the server in main file...\n");
    initialize_server();
}

int main(void) {
    printf("Hello, World!\n");
    start_server();
    // pointer_test();

    return 0;
}

