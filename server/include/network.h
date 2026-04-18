#pragma once

#include "types.h"

#define SOCKERR -1

void start_server(server_args *args, short port);

typedef enum { KEEP_ALIVE, CLOSE_FD } client_handle_otp;

client_handle_otp handle_client(int peer_fd);

int accept_conn(int listener_fd);