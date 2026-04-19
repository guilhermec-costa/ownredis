#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "types.h"

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    set_global_log_lvl(LOG_DEBUG);

    LOG_D("Server starting...\n");

    server _server = {
        .args = {argc, ++argv},
        .port = 6379,
        .loop = sel_event_loop,
    };

    start_server(_server);

    return EXIT_SUCCESS;
}