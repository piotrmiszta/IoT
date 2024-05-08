#include "list.h"
#include "allocator.h"

struct list_node
{
    struct list_node* next;
    struct list_node* prev;
    void* data;
};

typedef struct list
{
    struct list_node head;
    void (*dealloc)(void*);
    u32 size;
}list_t;

static struct list_node* allocate_list_node(void);
static inline void list_add_between(struct list_node* prev, struct list_node* next, struct list_node* entry);
static struct list_node* get_index(const struct list_node* head, u32 index);
static inline void list_del_entry(struct list_node* entry, void (*dealloc)(void*));

list_t* list_create(void (*dealloc)(void*))
{
    list_t* list = allocator_alloc(sizeof(list_t));
    if(list == NULL)
    {
        print_err("Cannot allocate memory!\n");
        return NULL;
    }
    else
    {
        list->head.next = &list->head;
        list->head.prev = &list->head;
        if(dealloc != NULL)
        {
            list->dealloc = dealloc;
        }
        else
        {
            list->dealloc = default_dealloc;
        }
        list->size = 0;
        return list;
    }
}

void list_destroy(list_t* list)
{
    while(!list_is_empty(list))
    {
        void* ptr = list_get_head(list);
        list->dealloc(ptr);
        list->size--;
    }
    allocator_free(list);
}

err_t list_add_head(list_t* list, void* data)
{
    struct list_node* node = allocate_list_node();
    if(node == NULL)
    {
        return -1; //TODO: create error description
    }
    list_add_between(&list->head, list->head.next, node);
    node->data = data;
    list->size++;
    return 0;
}

err_t list_add_tail(list_t* list, void* data)
{
    struct list_node* node = allocate_list_node();
    if(node == NULL)
    {
        return -1; //TODO: create error description
    }
    list_add_between(list->head.prev, &list->head, node);
    node->data = data;
    list->size++;
    return 0;
}

err_t list_add_index(list_t* list, void* data, u32 index)
{
    if(index >= list->size)
    {
        return -1; // TODO: create error description
    }
    struct list_node* node = allocate_list_node();
    if(node == NULL)
    {
        return -1; //TODO: create error description
    }
    /* iterate to index */
    struct list_node* prev = get_index(&list->head, index);
    list_add_between(prev, prev->next, node);
    node->data = data;
    list->size++;
    return 0;
}

void* list_get_head(list_t* list)
{
    void* data = list_get_ptr_head(list);
    list_del_head(list);
    return data;
}

void* list_get_tail(list_t* list)
{
    void* data = list_get_ptr_tail(list);
    list_del_tail(list);
    return data;
}

void* list_get_index(list_t* list, u32 index)
{
    if(index >= list->size)
    {
        return NULL;
    }
    void* data = list_get_ptr_index(list, index);
    list_del_index(list, index);
    return data;
}

void* list_get_ptr_head(list_t* list)
{
    return list->head.next->data;
}
void* list_get_ptr_tail(list_t* list)
{
    return list->head.prev->data;
}
void* list_get_ptr_index(list_t* list, u32 index)
{
    if(index >= list->size)
    {
        return NULL;
    }
    const struct list_node* node = get_index(&list->head, index);
    return node->data;

}

err_t list_del_head(list_t* list)
{
    list_del_entry(list->head.next, list->dealloc);
    list->size--;
    return 0;
}

err_t list_del_tail(list_t* list)
{
    list_del_entry(list->head.prev, list->dealloc);
    list->size--;
    return 0;
}

err_t list_del_index(list_t* list, u32 index)
{
    if(index >= list->size)
    {
        return -1; //TODO ERROR CODE
    }
    struct list_node* node = get_index(&list->head, index);
    list_del_entry(node, list->dealloc);
    list->size--;
    return 0;
}

bool list_is_empty(const list_t* list)
{
    return (list->head.next) == (&list->head);
}

u32 list_size(const list_t* list)
{
    return list->size;
}

__attribute__((weak))
void default_dealloc(void* arg)
{
    (void)arg;
}

static inline void list_add_between(struct list_node* prev, struct list_node* next, struct list_node* entry)
{
    entry->next = next;
    entry->prev = prev;
    prev->next = entry;
    next->prev = entry;
}

static struct list_node* get_index(const struct list_node* head, u32 index)
{
    assert_debug(head);
    struct list_node* ptr = head->next;
    for(u32 i = 0; i < index; i++)
    {
        ptr = ptr->next;
    }
    return ptr;
}

static inline void list_del_entry(struct list_node* entry, void (*dealloc)(void*))
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = NULL;
    entry->next = NULL;
    dealloc(entry->data);
    allocator_free(entry);
}

static struct list_node* allocate_list_node(void)
{
    struct list_node* node = allocator_alloc(sizeof(struct list_node));
    if(node == NULL)
    {
        print_err("Cannot allocate memory!\n");
        return NULL;
    }
    return node;
}
