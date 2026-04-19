#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

#include "network.h"
#include "types.h"

#define PORT 8000

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    set_global_log_lvl(LOG_DEBUG);

    LOG_D("Server starting at %s\n", "churros");
    server_args args = {argc, ++argv};
    start_server(&args, PORT);

    return EXIT_SUCCESS;
}