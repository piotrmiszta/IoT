#include "btree.h"
#include "allocator.h"

typedef struct btree_node
{
    void* data;
    struct btree_node* right;
    struct btree_node* left;
}btree_node_t;

typedef struct btree
{
    void (*dealloc)(void*);
    btree_node_t* root;
}btree_t;

static inline btree_node_t* btree_node_alloc(void);

static void __btree_for_each(btree_node_t* node, void* (*fun)(void* a));

static void __dealloc_all_node(btree_node_t* node, void (*fun)(void*));

static const btree_node_t* __btree_search(const btree_node_t* node, const void* data, i32 (*search_fun)(const void* a, const void* b));

btree_t* btree_create(void (*dealloc)(void*))
{
    btree_t* btree = allocator_alloc(sizeof(*btree));
    if(!btree)
    {
        print_err("Cannot allocate memory for new tree!");
        return NULL;
    }
    else
    {
        btree->root = allocator_alloc(sizeof(btree_node_t));
        btree->dealloc = dealloc;
        btree->root->data = NULL;
        btree->root->right = NULL;
        btree->root->left = NULL;
    }
    return btree;
}

void btree_destroy(btree_t* tree)
{
    __dealloc_all_node(tree->root, tree->dealloc);
    allocator_free(tree);
}

void* btree_get_data(const btree_node_t* node)
{
    return node->data;
}

btree_node_t* btree_get_root(btree_t* tree)
{
    return tree->root;
}

btree_node_t* btree_add_left(btree_node_t* root, void* data)
{
    btree_node_t* node = btree_node_alloc();
    if(node == NULL)
    {
        return NULL;
    }
    root->left = node;
    node->data = data;
    return node;
}

btree_node_t* btree_add_right(btree_node_t* root, void* data)
{
    btree_node_t* node = btree_node_alloc();
    if(node == NULL)
    {
        return NULL;
    }
    root->right = node;
    node->data = data;
    return node;
}

i32 btree_add_root_data(btree_t* tree, void* data)
{
    tree->root->data = data;
    return 0;
}

btree_node_t* btree_get_left(const btree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to get non exisitng child\n");
        return NULL;
    }
    return root->left;
}

btree_node_t* btree_get_right(const btree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to get non exisitng child\n");
        return NULL;
    }
    return root->right;
}

btree_node_t* btree_del_left(btree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to delete non exisitng child\n");
        return NULL;
    }
    btree_node_t* node = root->left;
    root->left = NULL;
    return node;
}

btree_node_t* btree_del_right(btree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to delete non exisitng child\n");
        return NULL;
    }
    btree_node_t* node = root->right;
    root->left = NULL;
    return node;
}

const btree_node_t* btree_search(const btree_t* tree, const void* data, i32 (*search_fun)(const void* a, const void* b))
{
    return __btree_search(tree->root, data, search_fun);
}

const btree_node_t* __btree_search(const btree_node_t* node, const void* data, i32 (*search_fun)(const void* a, const void* b))
{
    const btree_node_t* left = node->left;
    const btree_node_t* right = node->right;
    if(node->data)
    {
        if(search_fun(node->data, data))
        {
            return node;
        }
    }
    const btree_node_t* to_ret = __btree_search(left, data, search_fun);
    if(to_ret)
    {
        return to_ret;
    }
    to_ret = __btree_search(right, data, search_fun);
    if(to_ret)
    {
        return to_ret;
    }
    return NULL;
}

void btree_for_each(btree_t* tree, void* (*fun)(void* a))
{
    __btree_for_each(tree->root, fun);
}

static void __btree_for_each(btree_node_t* node, void* (*fun)(void* a))
{
    if(node != NULL)
    {
        btree_node_t* root = node;
        btree_node_t* left = node->left;
        btree_node_t* right = node->right;
        if(root->data)
            fun(root->data);
        __btree_for_each(left, fun);
        __btree_for_each(right, fun);
    }
}

static void __dealloc_all_node(btree_node_t* node, void (*fun)(void*))
{
    if(node != NULL)
    {
        btree_node_t* root = node;
        btree_node_t* left = node->left;
        btree_node_t* right = node->right;
        if(root->data)
            fun(root->data);
        __dealloc_all_node(left, fun);
        __dealloc_all_node(right, fun);
        allocator_free(root);
    }
}

static inline btree_node_t* btree_node_alloc(void)
{
    btree_node_t* node = allocator_alloc(sizeof(*node));
    if(node == NULL)
    {
        print_err("Cannot allocate memory for new node in btree!\n");
        return NULL;
    }
    node->data = NULL;
    node->right = NULL;
    node->left = NULL;
    return node;
}

btree_node_t* btree_add_child(btree_node_t* parent, void* data)
{
    if(btree_have_child(parent))
    {
        return btree_add_sibling(parent->left, data);
    }
    else
    {
        return btree_add_left(parent, data);
    }
}

btree_node_t* btree_add_sibling(btree_node_t* parent, void* data)
{
    btree_node_t* active = parent;
    while(active->right)
    {
        active = active->right;
    }
    return btree_add_right(active, data);
}

i32 btree_have_child(btree_node_t* parent)
{
    if(parent->left)
    {
        return true;
    }
    return false;
}
