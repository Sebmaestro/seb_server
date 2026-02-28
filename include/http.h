#ifndef HTTP_H
#define HTTP_H

void print_ascii(char *);
void parse_request_line(char *, char **, char **, char **);
void parse_request(char *, int *);

#endif