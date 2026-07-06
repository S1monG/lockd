// TODO: 
// log to specific file instead of stdout
// make thread safe with some kind of file lock
// add timestamp to log messages
// color? 

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

enum log_level_t {
    LOG_ERROR_T,
    LOG_WARN_T,
    LOG_INFO_T,
    LOG_DEBUG_T
};

#define LOG_ERROR(...) log(LOG_ERROR_T, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  log(LOG_WARN_T, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  log(LOG_INFO_T, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) log(LOG_DEBUG_T, __FILE__, __LINE__, __VA_ARGS__)

static inline const char *log_level_to_string(enum log_level_t level);
void log(int level, const char *file, int line, const char *fmt, ...);

#endif // LOGGER_H