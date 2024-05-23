#ifndef LIB_BINARY_TREE_H_
#define LIB_BINARY_TREE_H_

#include "types.h"

typedef struct btree btree_t;
typedef struct btree_node btree_node_t;

btree_t* btree_create(void (*dealloc)(void*));
btree_node_t* btree_get_root(btree_t* tree);
void btree_destroy(btree_t* tree);

btree_node_t* btree_add_left(btree_node_t* root, void* data);
btree_node_t* btree_add_right(btree_node_t* root, void* data);
i32 btree_add_root_data(btree_t* tree, void* data);

btree_node_t* btree_get_left(const btree_node_t* root);
btree_node_t* btree_get_right(const btree_node_t* root);

btree_node_t* btree_del_left(btree_node_t* root);
btree_node_t* btree_del_right(btree_node_t* root);
void* btree_get_data(const btree_node_t* node);

const btree_node_t* btree_search(const btree_t* tree, const void* data, i32 (*search_fun)(const void* a, const void* b));
void btree_for_each(btree_t* tree, void* (*fun)(void* a));

btree_node_t* btree_add_child(btree_node_t* parent, void* data);
btree_node_t* btree_add_sibling(btree_node_t* parent, void* data);

static inline btree_node_t* btree_get_child(const btree_node_t* root) {
    return btree_get_left(root);
}

static inline btree_node_t* btree_get_sibling(const btree_node_t* root) {
    return btree_get_right(root);
}

i32 btree_have_child(btree_node_t* parent);
#endif
