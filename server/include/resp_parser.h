#pragma once

#include <stddef.h>

#define RESP_TRMTOR "\r\n"

void resp_free_tokens(char** tokens);
char** resp_get_tokens(char* raw_resp);

#ifdef ESCAPE_RESP
void escape_resp(char* out, const char* in);
#endif