#ifndef LIB_QUEUE_H_
#define LIB_QUEUE_H_
#include "types.h"

typedef struct queue queue_t;

queue_t* queue_create(void (*dealloc)(void*));
void queue_destroy(queue_t* que);
i32 queue_push(queue_t* que, void* data);
void* queue_pop(queue_t* que);
u32 queue_size(const queue_t* que);
bool queue_is_empty(const queue_t* que);

#endif //LIB_QUEUE_H_
