#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_
#include <netinet/in.h>
#include "types.h"

typedef struct server
{
    i32 socket;
    struct sockaddr_in addr;
    socklen_t addr_len;
    bool run;
}server_t;

/*  */
i32 server_boot(server_t* server);
void server_close(void);
#endif
