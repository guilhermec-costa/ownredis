#include "logger.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

static void strnow(char* buf, size_t _size)
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buf, _size, "%d-%m-%Y %H:%M:%S", t);
}

static void log_msg_v(log_level_t msg_level, const char* label, FILE* out, const char* fmt,
                      va_list args)
{
    if (msg_level < g_logger.lvl)
        return;

    char timebuf[100];
    strnow(timebuf, sizeof(timebuf));
    fprintf(out, "[%s] [%s] ownredis: ", timebuf, label);

    vfprintf(out, fmt, args);
}

void log_debug(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_msg_v(LOG_DEBUG, "DEBUG", stdout, msg, args);
    va_end(args);
}

void log_info(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_msg_v(LOG_INFO, "INFO", stdout, msg, args);
    va_end(args);
}

void log_warn(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_msg_v(LOG_WARN, "WARN", stdout, msg, args);
    va_end(args);
}

void log_err(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_msg_v(LOG_ERROR, "ERROR", stdout, msg, args);
    va_end(args);
}

void set_g_log_lvl(log_level_t _l) { g_logger.lvl = _l; };

logger_t g_logger = {.err = log_err, .warn = log_warn, .info = log_info, .debug = log_debug};