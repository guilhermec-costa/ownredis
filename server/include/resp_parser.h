#pragma once

#include <stddef.h>

void parse_resp();

#ifdef ESCAPE_RESP
void escape_resp(char* out, const char* in);
#endif