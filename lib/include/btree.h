#ifndef LIB_BINARY_TREE_H_
#define LIB_BINARY_TREE_H_

#include "types.h"

typedef struct btree btree_t;
typedef struct btree_node btree_node_t;

btree_t* btree_create(void (*dealloc)(void*));
btree_node_t* btree_get_root(btree_t* tree);
void btree_destroy(btree_t* tree);

i32 btree_add_left(btree_node_t* root, void* data);
i32 btree_add_right(btree_node_t* root, void* data);
i32 btree_add_root_data(btree_t* tree, void* data);

btree_node_t* btree_get_left(const btree_node_t* root);
btree_node_t* btree_get_right(const btree_node_t* root);

btree_node_t* btree_del_left(btree_node_t* root);
btree_node_t* btree_del_right(btree_node_t* root);
void* btree_get_data(btree_node_t* node);

btree_node_t* btree_search(btree_t* tree, void* data, i32 (*search_fun)(void* a, void* b));
void btree_for_each(btree_t* tree, void* (*fun)(void* a));

#endif
