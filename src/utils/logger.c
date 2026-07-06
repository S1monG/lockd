#include "logger.h"

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

void log(int level, const char *file, int line, const char *fmt, ...)
{
    printf("[%s] %s:%d: ", log_level_to_string(level), file, line);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
