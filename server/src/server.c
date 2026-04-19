#include <err.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>

#include "logger.h"
#include "network.h"

void sel_event_loop(int listener_fd)
{
    fd_set resetfds, readfds;
    FD_ZERO(&resetfds);
    FD_SET(listener_fd, &resetfds);

    uint16_t max_fd = listener_fd;

    for (;;)
    {
        readfds = resetfds;
        // select modifies the fdsets in place
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("multiplexing failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < max_fd + 1; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                if (i == listener_fd)
                {
                    LOG_D("Client trying to connect...\n");
                    int peer_fd = accept_conn(listener_fd);
                    if (peer_fd == SOCKERR)
                        continue;

                    if (peer_fd > FD_SETSIZE)
                    {
                        LOG_E("Can not accept more connections. Limit: %d", FD_SETSIZE);
                        close(peer_fd);
                        continue;
                    }

                    FD_SET(peer_fd, &resetfds);
                    if (peer_fd > max_fd)
                    {
                        max_fd = peer_fd;
                    }
                }
                else
                {
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

void start_server(server _server)
{
    int sock_listener;
    exit_on_error((sock_listener = socket(AF_INET, SOCK_STREAM, 0)), "error on initializing server",
                  SOCKERR);

    int _opt_reuse = 1;
    exit_on_error(
        setsockopt(sock_listener, SOL_SOCKET, SO_REUSEADDR, &_opt_reuse, sizeof(_opt_reuse)),
        "setsockopt failed", SOCKERR);

    SA_IN _addr = {
        .sin_family = AF_INET, .sin_port = htons(_server.port), .sin_addr = {htonl(INADDR_ANY)}};

    exit_on_error(bind(sock_listener, (struct sockaddr*)&_addr, sizeof(_addr)),
                  "bind socket failed", SOCKERR);

    int conn_backlog = 5;
    exit_on_error(listen(sock_listener, conn_backlog), "sock listen failed", SOCKERR);

    LOG_D("Server listening on port %d. Waiting for connections...\n", _server.port);

    _server.loop(sock_listener);

    LOG_D("Ending server...\n");
    close(sock_listener);
}