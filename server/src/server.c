#include "types.h"
#include <err.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utils.h>

#define PORT 8000
#define SOCKERR -1

void handle_client(int);

static int accept_conn(int listener_socket) {
  int peer_fd;
  struct sockaddr_in peer_addr;
  socklen_t peer_len = sizeof(peer_addr);

  for (;;) {
    // bidirectional fullduplex TCP conn
    if ((peer_fd = accept(listener_socket, (struct sockaddr *)&peer_addr,
                          &peer_len)) == -1) {
      printf("Failed to accept client: %s", strerror(errno));
      return -1;
    };

    printf("Client accepted. Using sockfd %d\n", peer_fd);
    return peer_fd;
  }
}

void server_event_loop(int listener_fd) {
  fd_set cur_sockets, ready_sockets;
  FD_ZERO(&cur_sockets);
  FD_SET(listener_fd, &cur_sockets);

  for (;;) {
    ready_sockets = cur_sockets;
    // select modifies the fdsets in place
    if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
      perror("multiplexing failed");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (listener_fd) {
          // client trying to connect
          int peer_fd = accept_conn(listener_fd);
          if (peer_fd < 0)
            continue;

          FD_SET(peer_fd, &cur_sockets);
        } else {
          handle_client(i);
          FD_CLR(i, &cur_sockets);
        }
      }
    }
  }
}

void start_server(server_args args) {
  int sock_listener;
  exit_on_error((sock_listener = socket(AF_INET, SOCK_STREAM, 0)),
                "error on initializing server", SOCKERR);

  int _opt_reuse = 1;
  exit_on_error(setsockopt(sock_listener, SOL_SOCKET, SO_REUSEADDR, &_opt_reuse,
                           sizeof(_opt_reuse)),
                "setsockopt failed", SOCKERR);

  struct sockaddr_in _addr = {.sin_family = AF_INET,
                              .sin_port = htons(PORT),
                              .sin_addr = {htonl(INADDR_ANY)}};

  exit_on_error(bind(sock_listener, (struct sockaddr *)&_addr, sizeof(_addr)),
                "bind socket failed", SOCKERR);

  int conn_backlog = 5;
  exit_on_error(listen(sock_listener, conn_backlog), "sock listen failed",
                SOCKERR);

  printf("Server listening on port %d. Waiting for connections...\n", PORT);

  server_event_loop(sock_listener);

  printf("Ending server...\n");
  close(sock_listener);
}