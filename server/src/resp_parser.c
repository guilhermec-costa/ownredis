#include "logger.h"
#define _POSIX_C_SOURCE 200809L
#include "resp_parser.h"
#include <stdlib.h>
#include <string.h>

void resp_free_tokens(char** tokens)
{
    for (int i = 0; tokens[i] != NULL; i++)
    {
        free(tokens[i]);
    }
    free(tokens);
}

char** resp_get_tokens(char* raw_resp)
{
    char* saveptr;
    char* token = strtok_r(raw_resp, RESP_TRMTOR, &saveptr);

    short limit = 8;
    char** tokens = calloc(limit, sizeof(char*));

    int t = 0;
    while (token)
    {
        if (t >= limit)
        {
            limit *= 2;
            tokens = realloc(tokens, limit * sizeof(char*));
        }
        tokens[t++] = token;
        token = strtok_r(NULL, RESP_TRMTOR, &saveptr);
    }
    tokens[t] = NULL;
    return tokens;
}

#ifdef ESCAPE_RESP

void escape_resp(char* out, const char* in)
{
    size_t size = strlen(in) * 2;
    size_t j = 0;

    for (size_t i = 0; in[i] && j < size - 1; i++)
    {
        if (in[i] == '\n' || in[i] == '\r')
        {
            if (j + 2 >= size)
                break;
            out[j++] = '\\';
            out[j++] = (in[i] == '\n') ? 'n' : 'r';
        }
        else
        {
            out[j++] = in[i];
        }
    }
    out[j] = '\0';
}
#endif