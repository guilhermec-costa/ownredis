#include "resp_parser.h"
#include <string.h>

// #ifdef ESCAPE_RESP
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
// #endif