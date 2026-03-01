#include "../include/net.h"
#include "../include/http.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void write_response(int clientFD, char * method) {
    // printf("method is: %s\n", method);

    if(strcmp(method, "GET") == 0) {
        printf("Received a GET request\n");
    } else if (strcmp(method, "POST") == 0) {
        printf("Received a POST request\n");
    } 
    char * response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Length: 5\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "tjena";
    write(clientFD, response, strlen(response));
}

int read_loop(int clientFD, char * buffer, int * bodySize) {
    int bytesRead = 0;
    while(1) {
        //Read into buffer but move ahead the pointer the nr of bytes we have already read
        int n = read(clientFD, buffer+bytesRead, 1023-bytesRead);

        if (n < 0) {
            perror("Failed to read from client");
            return -1;
        }

        //Read is no longer reading
        if (n == 0) {
            printf("TJENAMORS\n");
            break;
        }
        bytesRead += n;
        buffer[bytesRead] = '\0';
        printf("Bytes read in loop: %d\n", bytesRead);

        char * headersEnd = strstr(buffer, "\r\n\r\n");
        
        if(headersEnd) {
            //Headers are read
            //We get the size of headers by subtracting distance of the addresses. +4 skips the \stuff
            int headersLen = headersEnd - buffer + 4;
            printf("Headers length: %d\n", headersLen);
            char * contentLength = strstr(buffer, "Content-Length: ");
            if(contentLength == NULL) {
                printf("No content length header found, assuming body size is 0.\n");
                *bodySize = 0;
                break;
            }
            // +16 to reach the actual size in that string
            contentLength = contentLength + 16;
            *bodySize = strtol(contentLength, NULL, 10);
            printf("bodysize: %d\n", *bodySize);
            int bodyRead = bytesRead - headersLen;

            if(bodyRead >= *bodySize) {
                //Body is fully read
                break;
            }
        } else {
            perror("Header could not be read i think\n");
        }
    }

    return bytesRead;
}


/**
* 
*/
void server_loop(int server_fd) {
    while (1) {
        printf("Listening for incoming connections on port 8080...\n");
        int clientFD = accept(server_fd, NULL, NULL);
        if (clientFD < 0) {
            perror("Failed to accept connection");
            continue;
        }

        char *buffer = malloc(1024);
        if (buffer == NULL) {
            perror("Failed to allocate buffer");
            close(clientFD);
            continue;
        }
        int bodySize = 0;
        int bytesRead = read_loop(clientFD, buffer, &bodySize);
        if (bytesRead < 0) {
            free(buffer);
            close(clientFD);
            continue;
        }

        buffer[bytesRead] = '\0';
        printf("Received data from client buffer: %s\n", buffer);
        printf("Bytes read: %d\n\n", bytesRead);

        // parsing the request todo
        char * method = NULL;
        char * path = NULL;
        char * version = NULL;
        parse_request(buffer, &bodySize, &method, &path, &version);

        printf("Method: %s\n", method);
        printf("Path: %s\n", path);
        printf("Version: %s\n", version);

        
        write_response(clientFD, method);
        free(buffer);
        close(clientFD);
    }
}


/**
* Initializes the server with sockets
*/
void initialize_server() {
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
    }

    server_loop(socketSwag);
    close(socketSwag);

}

void pointer_test() {
    int value = 5;
    int *pointer = &value;

    printf("*pointer: %d\n", *pointer);
    printf("&pointer: %p\n", &pointer);
    printf("pointer: %p\n", pointer);
}