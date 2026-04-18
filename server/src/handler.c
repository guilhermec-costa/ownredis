#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void handle_client(int fd) {
  printf("Handling socket %d\n", fd);

  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));

  for (;;) {
    int msg_len;
    if ((msg_len = recv(fd, buffer, sizeof(buffer), 0)) == -1) {
      printf("Failed reading client %d message: %s", fd, strerror(errno));
      continue;
    };

    printf("bytes read from socket %d: %d\n", fd, msg_len);
    printf("Message received: %s\n", buffer);

    if (strcmp(buffer, "PING\r\n") == 0) {
      if (write(fd, "PONG\n", 5) == -1) {
        printf("Failed writing to socket %d", fd);
        continue;
      };
    }
    printf("Buffer state: %s", buffer);
  }

  printf("Closing socket %d", fd);
  close(fd);
}
