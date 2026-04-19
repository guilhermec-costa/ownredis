#pragma once

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} log_level_t;

extern void set_g_log_lvl(log_level_t _l);

typedef void (*log_fn_t)(const char* msg, ...);

typedef struct
{
    log_level_t lvl;
    log_fn_t err;
    log_fn_t warn;
    log_fn_t info;
    log_fn_t debug;
} logger_t;

extern logger_t g_logger;

void log_debug(const char* fmt, ...);
void log_info(const char* fmt, ...);
void log_warn(const char* fmt, ...);
void log_err(const char* fmt, ...);

#define LOG_D(...) log_debug(__VA_ARGS__)
#define LOG_I(...) log_info(__VA_ARGS__)
#define LOG_W(...) log_warn(__VA_ARGS__)
#define LOG_E(...) log_err(__VA_ARGS__)