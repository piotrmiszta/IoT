#include <stdio.h>
#include "server.h"
#include "logger.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
    logger_init(NULL, NULL);
    LOG_THREAD("main");
    struct server s;
    server_boot(&s);
    server_close();
    logger_close();
}