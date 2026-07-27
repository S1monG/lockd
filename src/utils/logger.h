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

#define LOG_TARGET_STDOUT() log_set_target(stdout, NULL)
#define LOG_TARGET_STDERR() log_set_target(stderr, NULL)
#define LOG_TARGET_FILE(path) log_set_target(NULL, path)

#define LOG_ERROR(...) log_log(LOG_ERROR_T, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  log_log(LOG_WARN_T, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  log_log(LOG_INFO_T, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) log_log(LOG_DEBUG_T, __FILE__, __LINE__, __VA_ARGS__)

void log_set_target(FILE *target, const char *path);
void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif // LOGGER_H