#include "queue.h"
#include "allocator.h"

__attribute__((weak))
void queue_default_dealloc(void* arg)
{
    (void)arg;
}

struct queue_node
{
    struct queue_node* next;
    void* data;
};

typedef struct queue
{
    struct queue_node* head;
    struct queue_node* tail;
    void (*dealloc)(void*);
}queue_t;

queue_t* queue_create(void (*dealloc)(void*))
{
    queue_t* que = allocator_alloc(sizeof(struct queue));
    if(que == NULL)
    {
        print_err("Cannot allocate memory for queue\n");
        return NULL;
    }
    que->head = NULL;
    que->tail = NULL;
    if(dealloc == NULL)
    {
        que->dealloc = queue_default_dealloc;
    }
    else
    {
        que->dealloc = dealloc;
    }
    return que;
}

void queue_destroy(queue_t* que)
{
    while(!queue_is_empty(que))
    {
        void* data = queue_pop(que);
        que->dealloc(data);
    }
    allocator_free(que);
}

i32 queue_push(queue_t* que, void* data)
{
    struct queue_node* node = allocator_alloc(sizeof(struct queue_node));
    if(node == NULL)
    {
        print_err("Cannot allocate memory for queue node\n");
        return -1;
    }
    node->data = data;
    node->next = NULL;
    if(que->tail == NULL)
    {
        que->tail = node;
        que->head = node;

    }
    else
    {
        que->tail->next = node;
        que->tail = node;
    }
    return 0;
}

void* queue_pop(queue_t* que)
{
    void* ret = NULL;
    if(que->head == NULL)
    {
        print_err("Try to pop from empty que\n");
    }
    else if(que->head == que->tail)
    {
        ret = que->head->data;
        allocator_free(que->head);
        que->head = NULL;
        que->tail = NULL;
    }
    else
    {
        struct queue_node* old = que->head;
        que->head = que->head->next;
        ret = old->data;
        allocator_free(old);
    }
    return ret;
}

u32 queue_size(const queue_t* que)
{
    if(que->head == NULL)
    {
        return 0;
    }
    u32 size = 1;
    struct queue_node* node = que->head;
    while(node != que->tail)
    {
        node = node->next;
        size++;
    }
    return size;
}

bool queue_is_empty(const queue_t* que)
{
    return que->head == NULL;
}
