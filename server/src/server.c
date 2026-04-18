#include <err.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>

#include "network.h"

void sel_event_loop(int listener_fd)
{
    fd_set resetfds, readfds;
    FD_ZERO(&resetfds);
    FD_SET(listener_fd, &resetfds);

    for (;;)
    {
        readfds = resetfds;
        // select modifies the fdsets in place
        if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("multiplexing failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                if (i == listener_fd)
                {
                    printf("Client trying to connect...\n");
                    int peer_fd = accept_conn(listener_fd);
                    if (peer_fd == SOCKERR)
                        continue;

                    FD_SET(peer_fd, &resetfds);
                }
                else
                {
                    printf("Message here on fd %d\n", i);
                    int otp = handle_client(i);
                    switch (otp)
                    {
                    case CLOSE_FD:
                    {
                        close(i);
                        FD_CLR(i, &resetfds);
                    }
                    }
                }
            }
        }
    }
}

void start_server(server_args* args, short port)
{
    int sock_listener;
    exit_on_error((sock_listener = socket(AF_INET, SOCK_STREAM, 0)), "error on initializing server",
                  SOCKERR);

    int _opt_reuse = 1;
    exit_on_error(
        setsockopt(sock_listener, SOL_SOCKET, SO_REUSEADDR, &_opt_reuse, sizeof(_opt_reuse)),
        "setsockopt failed", SOCKERR);

    SA_IN _addr = {.sin_family = AF_INET, .sin_port = htons(port), .sin_addr = {htonl(INADDR_ANY)}};

    exit_on_error(bind(sock_listener, (struct sockaddr*)&_addr, sizeof(_addr)),
                  "bind socket failed", SOCKERR);

    int conn_backlog = 5;
    exit_on_error(listen(sock_listener, conn_backlog), "sock listen failed", SOCKERR);

    printf("Server listening on port %d. Waiting for connections...\n", port);

    sel_event_loop(sock_listener);

    printf("Ending server...\n");
    close(sock_listener);
}