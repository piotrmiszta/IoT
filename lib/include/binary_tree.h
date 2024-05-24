#ifndef LIB_BINARY_TREE_H_
#define LIB_BINARY_TREE_H_

#include "types.h"

typedef struct binary_tree binary_tree_t;
typedef struct binary_tree_node binary_tree_node_t;

binary_tree_t* binary_tree_create(void (*dealloc)(void*));
binary_tree_node_t* binary_tree_get_root(binary_tree_t* tree);
void binary_tree_destroy(binary_tree_t* tree);

binary_tree_node_t* binary_tree_add_left(binary_tree_node_t* root, void* data);
binary_tree_node_t* binary_tree_add_right(binary_tree_node_t* root, void* data);
i32 binary_tree_add_root_data(binary_tree_t* tree, void* data);

binary_tree_node_t* binary_tree_get_left(const binary_tree_node_t* root);
binary_tree_node_t* binary_tree_get_right(const binary_tree_node_t* root);

binary_tree_node_t* binary_tree_del_left(binary_tree_node_t* root);
binary_tree_node_t* binary_tree_del_right(binary_tree_node_t* root);
void* binary_tree_get_data(const binary_tree_node_t* node);

const binary_tree_node_t* binary_tree_search(const binary_tree_t* tree, const void* data, i32 (*search_fun)(const void* a, const void* b));
void binary_tree_for_each(binary_tree_t* tree, void* (*fun)(void* a));

binary_tree_node_t* binary_tree_add_child(binary_tree_node_t* parent, void* data);
binary_tree_node_t* binary_tree_add_sibling(binary_tree_node_t* parent, void* data);

static inline binary_tree_node_t* binary_tree_get_child(const binary_tree_node_t* root) {
    return binary_tree_get_left(root);
}

static inline binary_tree_node_t* binary_tree_get_sibling(const binary_tree_node_t* root) {
    return binary_tree_get_right(root);
}

i32 binary_tree_have_child(binary_tree_node_t* parent);
#endif
