#include "network.h"

#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "logger.h"
#include "resp_parser.h"
#include "types.h"

int accept_conn(int listener_socket)
{
    int peer_fd;
    SA_IN peer_addr;
    socklen_t peer_len = sizeof(peer_addr);

    // bidirectional fullduplex TCP conn
    if ((peer_fd = accept(listener_socket, (struct sockaddr*)&peer_addr, &peer_len)) == SOCKERR)
    {
        LOG_E("Failed to accept client: %s", strerror(errno));
        return SOCKERR;
    };

    LOG_D("Client accepted. Using sockfd %d\n", peer_fd);
    return peer_fd;
}

#define CLIENT_BUF_SIZE 1024

client_res_t handle_client(int fd)
{
    LOG_D("Handling socket %d\n", fd);

    char resp_buf[1024];
    memset(resp_buf, 0, sizeof(resp_buf));

    int msg_len;
    msg_len = recv(fd, resp_buf, sizeof(resp_buf), 0);

    if (msg_len == -1)
    {
        LOG_E("Failed reading client %d message: %s", fd, strerror(errno));
        return CLOSE_FD;
    }

    if (msg_len == SOCK_CLOSED)
    {
        LOG_D("Client closed connection. Closing fd on server side\n");
        return CLOSE_FD;
    }

    LOG_D("bytes read from socket %d: %d\n", fd, msg_len);

#ifdef ESCAPE_RESP
    char escp_buf[CLIENT_BUF_SIZE * 2 + 1];
    memset(escp_buf, 0, sizeof(escp_buf));
    escape_resp(escp_buf, resp_buf);
    LOG_D("Escaped buffer state: %s\n", escp_buf);
#endif

    char** tokens = resp_get_tokens(resp_buf);
    send(fd, "+OK\r\n", 5, 0);
    resp_free_tokens(tokens);
    return KEEP_ALIVE;
}