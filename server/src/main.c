#include "types.h"
#include <stdio.h>
#include <stdlib.h>

extern void start_server(server_args args, short port);

#define PORT 8000

int main(int argc, char **argv) {
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  server_args args = {argc, ++argv};
  start_server(args, PORT);

  return EXIT_SUCCESS;
}