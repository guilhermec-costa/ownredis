#include <stdio.h>
#include <stdlib.h>

#include "network.h"
#include "types.h"

#define PORT 8000

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    server_args args = {argc, ++argv};
    start_server(&args, PORT);

    return EXIT_SUCCESS;
}