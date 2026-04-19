#include "network.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "types.h"

int accept_conn(int listener_socket)
{
    int peer_fd;
    SA_IN peer_addr;
    socklen_t peer_len = sizeof(peer_addr);

    // bidirectional fullduplex TCP conn
    if ((peer_fd = accept(listener_socket, (struct sockaddr*)&peer_addr, &peer_len)) == SOCKERR)
    {
        fprintf(stderr, "Failed to accept client: %s", strerror(errno));
        return SOCKERR;
    };

    printf("Client accepted. Using sockfd %d\n", peer_fd);
    return peer_fd;
}

client_handle_otp handle_client(int fd)
{
    printf("Handling socket %d\n", fd);

    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    int msg_len;
    msg_len = recv(fd, buffer, sizeof(buffer), 0);

    if (msg_len == -1)
    {
        fprintf(stderr, "Failed reading client %d message: %s", fd, strerror(errno));
        return CLOSE_FD;
    }

    if (msg_len == SOCK_CLOSED)
    {
        printf("Client closed connection. Closing fd on server side\n");
        return CLOSE_FD;
    }

    printf("bytes read from socket %d: %d\n", fd, msg_len);
    printf("Buffer state: %s", buffer);

    if (strcmp(buffer, "PING\r\n") == 0)
    {
        const char* pong_buf = "+PONG\r\n";
        if (send(fd, pong_buf, strlen(pong_buf), 0) == SOCKERR)
        {
            fprintf(stderr, "Failed writing to socket %d", fd);
            return CLOSE_FD;
        }
    }

    if (strcmp(buffer, "quit\r\n") == 0)
    {
        return CLOSE_FD;
    }

    return KEEP_ALIVE;
}