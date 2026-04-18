#pragma once

#include <netinet/in.h>

typedef struct server_args {
  int count;
  char** args;
} server_args;

typedef struct sockaddr_in SA_IN;