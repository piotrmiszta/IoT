#include "logger.h"
#include "allocator.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include "types.h"
#include "queue.h"

#define BLACK               "\033[0;30m"
#define RED                 "\033[0;31m"
#define GREEN               "\033[0;32m"
#define ORANGE              "\033[0;33m"
#define BLUE                "\033[0;34m"
#define PURPLE              "\033[0;35m"
#define CYAN                "\033[0;36m"
#define LGRAY               "\033[0;37m"
#define DGRAY               "\033[1;30m"
#define LRED                "\033[1;31m"
#define LGREEN              "\033[1;32m"
#define YELLOW              "\033[1;33m"
#define LBLUE               "\033[1;34m"
#define LPURPLE             "\033[1;35m"
#define LCYAN               "\033[1;36m"
#define WHITE               "\033[1;37m"
#define RESET               "\x1B[0m"

#define DEBUG_COLOR         BLUE
#define TRACE_COLOR         GREEN
#define WARNING_COLOR       PURPLE
#define INFO_COLOR          GREEN
#define FATAL_COLOR         RED
#define ERROR_COLOR         RED
#define THREAD_COLOR        LCYAN
#define TRHREAD_INFO_COLOR  ORANGE
#define FILE_INFO_COLOR     LGRAY
#define TIME_INFO_COLOR     LPURPLE
#define SEP_COLOR           GREEN


static FILE* ofile;

struct log_msg
{
    char file[200];
    char func[200];
    i32 line;
    i32 thread;
    u32 log_level;
    char msg[1000];
};

static queue_t* que;
static pthread_mutex_t mtx;
static sem_t sem_full;
static sem_t sem_empty;
static pthread_t thread;
static bool run = 0;

static void log_print(struct log_msg* msg);
static void* logger_thread(void*);

static inline void print_time(FILE* stream);
/**/
i32 logger_log( const char* file,
                const char* func,
                i32 line,
                i32 thread,
                u32 log_level,
                const char* fmt,
                ...)
{
    struct log_msg* msg = allocator_alloc(sizeof(struct log_msg));
    memcpy(msg->file, file, sizeof(msg->file));
    memcpy(msg->func, func, sizeof(msg->func));
    msg->line = line;
    msg->thread = thread;
    msg->log_level = log_level;
    va_list list;
    va_start(list, fmt);
    vsnprintf(msg->msg, sizeof(msg->msg), fmt, list);
    va_end(list);

    sem_wait(&sem_empty);
    pthread_mutex_lock(&mtx);
    queue_push(que, msg);
    pthread_mutex_unlock(&mtx);
    sem_post(&sem_full);
    return 0;
}

i32 logger_init(const char* filename, const char* desc)
{
    if(filename)
    {
        ofile = fopen(filename, "w");
    }
    else
    {
        ofile = stdout;
    }
    run = 1;
    pthread_mutex_init(&mtx, NULL);
    sem_init(&sem_empty, 0, 10);
    sem_init(&sem_full, 0, 0);
    pthread_create(&thread, NULL, logger_thread, NULL);
    que = queue_create(NULL);
    return 0;
}

i32 logger_close(void)
{
    run = 0;
    pthread_join(thread, NULL);
    sem_post(&sem_empty);
    sem_destroy(&sem_empty);
    pthread_mutex_destroy(&mtx);
    fclose(ofile);
    queue_destroy(que);
    return 0;
}


static void log_print(struct log_msg* msg)
{
    fprintf(ofile, "[");
    print_time(ofile);
    fprintf(ofile, "]");
    switch(msg->log_level)
    {
        case LOGGER_DEBUG:
            fprintf(ofile, DEBUG_COLOR"[ DEBUG ]"RESET);
            break;
        case LOGGER_ERROR:
            fprintf(ofile, ERROR_COLOR"[ ERROR ]"RESET);
            break;
        case LOGGER_FATAL:
            fprintf(ofile, FATAL_COLOR"[ FATAL ]"RESET);
            break;
        case LOGGER_WARNING:
            fprintf(ofile, WARNING_COLOR"[ WARNI ]"RESET);
            break;
        case LOGGER_INFO:
            fprintf(ofile, INFO_COLOR"[  INFO ]"RESET);
            break;
        case LOGGER_TRACE:
            fprintf(ofile, TRACE_COLOR"[ TRACE ]"RESET);
            break;
        case LOGGER_THREAD:
            fprintf(ofile, THREAD_COLOR"[ THRED ]"RESET);
            break;
        default:
            fprintf(ofile, "[ UNKNW ]");
            break;
    }
    fprintf(ofile, FILE_INFO_COLOR" %s:%u - %s "RESET   \
                   TRHREAD_INFO_COLOR" THREAD: %u "     \
                   SEP_COLOR" :: "RESET  " %s\n",       \
                   msg->file, msg->line, msg->func, msg->thread, msg->msg);
}

static void* logger_thread(void* arg)
{
    (void)arg;
    while(run)
    {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mtx);
        struct log_msg* msg = queue_pop(que);
        pthread_mutex_unlock(&mtx);
        sem_post(&sem_empty);
        log_print(msg);
        allocator_free(msg);
    }
    pthread_mutex_lock(&mtx);
    while(!queue_is_empty(que))
    {
        struct log_msg* msg = queue_pop(que);
        log_print(msg);
        allocator_free(msg);
    }
    pthread_mutex_unlock(&mtx);
    return NULL;
}

static inline void print_time(FILE* stream) {
    time_t result = time(NULL);
    char * buff = asctime(gmtime(&result));
    buff[strlen(buff) - 1] = '\0';
    fprintf(stream, TIME_INFO_COLOR"%s"RESET, buff);
}
