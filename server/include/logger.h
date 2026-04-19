#pragma once

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} log_level;

extern void set_global_log_lvl(log_level _l);

typedef void (*log_fn)(const char* msg, ...);

typedef struct
{
    log_level lvl;
    log_fn err;
    log_fn warn;
    log_fn info;
    log_fn debug;
} logger_t;

extern logger_t logger;

void log_debug(const char* fmt, ...);
void log_info(const char* fmt, ...);
void log_warn(const char* fmt, ...);
void log_err(const char* fmt, ...);

#define LOG_D(...) log_debug(__VA_ARGS__)
#define LOG_I(...) log_info(__VA_ARGS__)
#define LOG_W(...) log_warn(__VA_ARGS__)
#define LOG_E(...) log_err(__VA_ARGS__)