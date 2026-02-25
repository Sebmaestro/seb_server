#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

void accept_connection() {
    int socketSwag = socket(AF_INET, SOCK_STREAM, 0);
    if (socketSwag < 0) {
        perror("Failed to create socket");
        return;
    }
    printf("Socket created successfully with descriptor: %d\n", socketSwag);

    int opt = 1;
if (setsockopt(socketSwag, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEADDR failed");
    return;
}
    

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Here we create a pointer to the address of serverAddress 
    // and casts sockaddr_in to sockaddr, which is what bind expects. 
    struct sockaddr * sockaddr_pointer = (struct sockaddr*)&serverAddress;


    int bindResult = bind(socketSwag, sockaddr_pointer, sizeof(serverAddress));
    if (bindResult < 0) {
        perror("Failed to bind socket");
        return;
    }

    int listenResult = listen(socketSwag, 10);
    if (listenResult < 0) {
        perror("Failed to listen on socket");
        return;
    } else {
        while (1) {
            printf("Listening for incoming connections on port 8080...\n");
            int clientFD = accept(socketSwag, NULL, NULL);
            if (clientFD < 0) {
                perror("Failed to accept connection");
                continue;
            } else {
                char * buffer = malloc(1024);
                int bytesRead = read(clientFD, buffer, 1024);
                if (bytesRead < 0) {
                    perror("Failed to read from client");
                    free(buffer);
                    close(clientFD);
                    continue;
                } 
                printf("Received data from client buffer: %s\n", buffer);
                printf("Bytes read: %d\n\n", bytesRead);
                free(buffer);
                close(clientFD);
            }
        }
    }
    close(socketSwag);

}

void pointer_test() {
    int value = 5;
    int *pointer = &value;

    printf("*pointer: %d\n", *pointer);
    printf("&pointer: %p\n", &pointer);
    printf("pointer: %p\n", pointer);
}