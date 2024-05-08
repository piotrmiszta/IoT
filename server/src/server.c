#include "server.h"
#include "logger.h"
#include "list.h"
#include <unistd.h>
#define SERVER_MAX_LISTEN       5

struct client_stub_struct
{
    pthread_t thread;
};

static list_t* clients_threads;
static pthread_t listen_thread;
static void* client_stub(void*);
static void* server_listen(void* arg);

i32 server_boot(server_t* server)
{
    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server->socket < 0)
    {
        LOG_FATAL("Cannot create socket for server, calling exit");
        return -1;
    }
    server->addr.sin_addr.s_addr = INADDR_ANY;
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(8080);
    i32 ret = bind(server->socket,
                   (struct sockaddr*)&server->addr,
                   sizeof(server->addr));
    if(ret < 0)
    {
        LOG_FATAL("Cannot bind socket, calling exit");
        return -1;
    }
    /* after succesful booted create new thread that listen and accepting new client */
    server->run = true;
    clients_threads = list_create(NULL); //change function to dealloc
    pthread_create(&listen_thread, NULL, server_listen, server);
    return 0;
}

static void* server_listen(void* arg)
{
    LOG_THREAD("Server Listen");
    server_t* server = arg;
    struct sockaddr* addr = (struct sockaddr*)&server->addr;
    socklen_t* addr_len = &server->addr_len;
    i32 fd = server->socket;
    LOG_DEBUG("Starting listening");
    if(listen(fd, SERVER_MAX_LISTEN) < 0)
    {
        LOG_ERROR("Cannot listen! calling exit");
        return NULL;
    }
    while(server->run)
    {
        i32 new_socket = accept(fd, addr, addr_len);
        if(new_socket < 0)
        {
            LOG_ERROR("Cannot accept new client");
        }
        else
        {
            /* create new thread*/
            struct client_stub_struct* client = allocator_alloc(sizeof(struct client_stub_struct));
            if(client == NULL)
            {
                LOG_ERROR("Cannot allocate memory for new client!");
            }
            else
            {
                list_add_tail(clients_threads, client);
                pthread_create(&client->thread, NULL, client_stub, NULL);
            }
        }
    }
    while(!list_is_empty(clients_threads))
    {
        struct client_stub_struct* client = list_get_head(clients_threads);
        pthread_join(client->thread, NULL);
    }
    return NULL;
}

static void* client_stub(void* arg)
{
    (void)arg;
    LOG_THREAD("client handler");
    while(1)
    {
        LOG_TRACE("Client thread executed");
        sleep(1);
    }
    return NULL;
}

void server_close(void)
{
    pthread_join(listen_thread, NULL);
}

