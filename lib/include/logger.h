#ifndef LIB_LOGGER_H_
#define LIB_LOGGER_H_
#include <pthread.h>
#include "allocator.h"
#include "types.h"

/*  TODO: create logger that will be faster to log in binary form, and create decoder for it */
/*
 *  currently logger work as default output to check trace etc.
 *  each thread should have entry with init to identify new thread
 *  macro LOG_THREAD(name) do it
 */

enum
{
    LOGGER_DEBUG = 0,
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_FATAL,
    LOGGER_ERROR,
    LOGGER_TRACE,
    LOGGER_THREAD,
};

i32 logger_log( const char* file,
                const char* func,
                i32 line,
                i32 thread,
                u32 log_level,
                const char* fmt,
                ...);

#define _logger_log(type, ...)      logger_log(__FILE__, __func__, __LINE__, pthread_self(), type, __VA_ARGS__)
#define LOG_DEBUG(...)              _logger_log(LOGGER_DEBUG, __VA_ARGS__)
#define LOG_INFO(...)               _logger_log(LOGGER_INFO, __VA_ARGS__)
#define LOG_WARNING(...)            _logger_log(LOGGER_WARNING, __VA_ARGS__)
#define LOG_FATAL(...)              _logger_log(LOGGER_FATAL, __VA_ARGS__)
#define LOG_ERROR(...)              _logger_log(LOGGER_ERROR, __VA_ARGS__)
#define LOG_TRACE(...)              _logger_log(LOGGER_TRACE, __VA_ARGS__)
#define LOG_THREAD(name)            _logger_log(LOGGER_THREAD, "New thread started %s", name)

i32 logger_init(const char* filename, const char* desc);
i32 logger_close(void);

#endif
