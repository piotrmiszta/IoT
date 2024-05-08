#include "connection.h"
#include "logger.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

i32 client_connect(void)
{
    int status, fd;
    struct sockaddr_in server;
    char* msg = "msg";
    char buffer[1024] = { 0 };
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("Can't create socket!");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)
        <= 0) {
        LOG_ERROR("Invalid address!");
        return -1;
    }

    if ((status = connect(fd, (struct sockaddr*)&server,
                   sizeof(server))) < 0) {
        LOG_ERROR("Can't connect to server!");
        return -1;
    }
    while(1)
    {
        sleep(1);
        ssize_t v = send(fd, msg, strlen(msg), 0);
        printf("msg message sent %ld\n", v);
        read(fd, buffer,
                    1024 - 1);
        printf("%s\n", buffer);
    }
    close(fd);
    return 0;
}
