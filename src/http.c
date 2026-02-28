#include "../include/http.h"
#include <stdio.h>
#include <string.h>


void print_ascii(char * buffer) {
    printf("Printing ascii code inc...:\n");
    for (size_t i = 0; i < strlen(buffer); i++) {
        printf("Ascii code:%d\n", buffer[i]);
    }
    printf("\n");
}


void parse_request_line(char * requestLine, char ** method, char ** path, char ** version) {
    *method = strtok(requestLine, " ");
    *path = strtok(NULL, " ");
    *version = strtok(NULL, " ");
}


void parse_request(char * buffer, int * bodySize) {
    printf("This is in the parse_req function: \n%s\n", buffer);

    char * requestLine = buffer; 
    char * requestLineEnd = strstr(buffer, "\r\n");
    *requestLineEnd = '\0';
    printf("reqline: \n%s\n\n", requestLine);

    
    char * header = requestLineEnd+2;
    
    char * headerEnd = strstr(header, "\r\n\r\n");
    *headerEnd = '\0';
    printf("header: %s\n\n", header);

    char * body = headerEnd+4;
    printf("body: %s\n\n", body);

    //printf("halloj %s\n", parse_method(requestLine));
    char * method = NULL;
    char * path = NULL;
    char * version = NULL;
    parse_request_line(requestLine, &method, &path, &version);

    printf("Method: %s\n", method);
    printf("Path: %s\n", path);
    printf("Version: %s\n", version);
}

