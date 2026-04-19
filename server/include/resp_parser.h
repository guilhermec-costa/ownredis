#pragma once

#include <stddef.h>

#define RESP_TRMTOR "\r\n"

void resp_free_tokens(char** tokens);

typedef char** resp_tokens_t;
resp_tokens_t resp_get_tokens(char* raw_resp);

typedef enum
{
    RESP_SIMPLE_STR,
    RESP_BULK_STR,
    RESP_ARRAY
} resp_type_t;

#ifdef ESCAPE_RESP
void escape_resp(char* out, const char* in);
#endif