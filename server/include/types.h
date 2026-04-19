#pragma once

#include <netinet/in.h>

typedef struct server_args
{
    int count;
    char** args;
} server_args;

typedef void (*loop_interface)(int fd);

void sel_event_loop(int listener_fd);

typedef struct
{
    server_args args;
    short port;
    loop_interface loop;
} server;

typedef struct sockaddr_in SA_IN;