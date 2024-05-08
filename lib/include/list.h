#ifndef LIB_LIST_H_
#define LIB_LIST_H_
#include "types.h"

typedef struct list list_t;

void default_dealloc(void* arg);

list_t* list_create(void (*dealloc)(void*));
void list_destroy(list_t* list);

err_t list_add_head(list_t* list, void* data);
err_t list_add_tail(list_t* list, void* data);
err_t list_add_index(list_t* list, void* data, u32 index);

void* list_get_head(list_t* list);
void* list_get_tail(list_t* list);
void* list_get_index(list_t* list, u32 index);

void* list_get_ptr_head(list_t* list);
void* list_get_ptr_tail(list_t* list);
void* list_get_ptr_index(list_t* list, u32 index);

err_t list_del_head(list_t* list);
err_t list_del_tail(list_t* list);
err_t list_del_index(list_t* list, u32 index);

bool list_is_empty(const list_t* list);
u32 list_size(const list_t* list);

#endif //LIB_LIST_H_
