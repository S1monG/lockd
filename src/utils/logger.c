#include "logger.h"

static FILE *log_target = NULL;

static inline const char *log_level_to_string(enum log_level_t level)
{
    switch (level) {
        case LOG_ERROR_T: return "ERROR";
        case LOG_WARN_T: return "WARN";
        case LOG_INFO_T: return "INFO";
        case LOG_DEBUG_T: return "DEBUG";
        default: return "UNKNOWN";
    }
}

void log_set_target(FILE *target, const char *path)
{
    if (target != NULL) {
        log_target = target;
        return;
    }

    if (path != NULL) {
        FILE *file = fopen(path, "a");
        if (file == NULL) {
            fprintf(stderr, "Failed to open log file: %s\n", path);
            return;
        }
        log_target = file;
    }
}

void log_log(int level, const char *file, int line, const char *fmt, ...)
{
    if (log_target == NULL) {
        return;
    }

    fprintf(log_target, "[%s] %s:%d: ", log_level_to_string(level), file, line);
    va_list args;
    va_start(args, fmt);
    vfprintf(log_target, fmt, args);
    va_end(args);
    fprintf(log_target, "\n");
}
