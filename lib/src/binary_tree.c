#include "binary_tree.h"
#include "allocator.h"

typedef struct binary_tree_node
{
    void* data;
    struct binary_tree_node* right;
    struct binary_tree_node* left;
}binary_tree_node_t;

typedef struct binary_tree
{
    void (*dealloc)(void*);
    binary_tree_node_t* root;
}binary_tree_t;

static inline binary_tree_node_t* binary_tree_node_alloc(void);

static void __binary_tree_for_each(binary_tree_node_t* node, void* (*fun)(void* a));

static void __dealloc_all_node(binary_tree_node_t* node, void (*fun)(void*));

static const binary_tree_node_t* __binary_tree_search(const binary_tree_node_t* node, const void* data, i32 (*search_fun)(const void* a, const void* b));

binary_tree_t* binary_tree_create(void (*dealloc)(void*))
{
    binary_tree_t* binary_tree = allocator_alloc(sizeof(*binary_tree));
    if(!binary_tree)
    {
        print_err("Cannot allocate memory for new tree!");
        return NULL;
    }
    else
    {
        binary_tree->root = allocator_alloc(sizeof(binary_tree_node_t));
        binary_tree->dealloc = dealloc;
        binary_tree->root->data = NULL;
        binary_tree->root->right = NULL;
        binary_tree->root->left = NULL;
    }
    return binary_tree;
}

void binary_tree_destroy(binary_tree_t* tree)
{
    __dealloc_all_node(tree->root, tree->dealloc);
    allocator_free(tree);
}

void* binary_tree_get_data(const binary_tree_node_t* node)
{
    return node->data;
}

binary_tree_node_t* binary_tree_get_root(binary_tree_t* tree)
{
    return tree->root;
}

binary_tree_node_t* binary_tree_add_left(binary_tree_node_t* root, void* data)
{
    binary_tree_node_t* node = binary_tree_node_alloc();
    if(node == NULL)
    {
        return NULL;
    }
    root->left = node;
    node->data = data;
    return node;
}

binary_tree_node_t* binary_tree_add_right(binary_tree_node_t* root, void* data)
{
    binary_tree_node_t* node = binary_tree_node_alloc();
    if(node == NULL)
    {
        return NULL;
    }
    root->right = node;
    node->data = data;
    return node;
}

i32 binary_tree_add_root_data(binary_tree_t* tree, void* data)
{
    tree->root->data = data;
    return 0;
}

binary_tree_node_t* binary_tree_get_left(const binary_tree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to get non exisitng child\n");
        return NULL;
    }
    return root->left;
}

binary_tree_node_t* binary_tree_get_right(const binary_tree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to get non exisitng child\n");
        return NULL;
    }
    return root->right;
}

binary_tree_node_t* binary_tree_del_left(binary_tree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to delete non exisitng child\n");
        return NULL;
    }
    binary_tree_node_t* node = root->left;
    root->left = NULL;
    return node;
}

binary_tree_node_t* binary_tree_del_right(binary_tree_node_t* root)
{
    if(root == NULL)
    {
        print_err("Try to delete non exisitng child\n");
        return NULL;
    }
    binary_tree_node_t* node = root->right;
    root->left = NULL;
    return node;
}

const binary_tree_node_t* binary_tree_search(const binary_tree_t* tree, const void* data, i32 (*search_fun)(const void* a, const void* b))
{
    return __binary_tree_search(tree->root, data, search_fun);
}

const binary_tree_node_t* __binary_tree_search(const binary_tree_node_t* node, const void* data, i32 (*search_fun)(const void* a, const void* b))
{
    const binary_tree_node_t* left = node->left;
    const binary_tree_node_t* right = node->right;
    if(node->data)
    {
        if(search_fun(node->data, data))
        {
            return node;
        }
    }
    const binary_tree_node_t* to_ret = __binary_tree_search(left, data, search_fun);
    if(to_ret)
    {
        return to_ret;
    }
    to_ret = __binary_tree_search(right, data, search_fun);
    if(to_ret)
    {
        return to_ret;
    }
    return NULL;
}

void binary_tree_for_each(binary_tree_t* tree, void* (*fun)(void* a))
{
    __binary_tree_for_each(tree->root, fun);
}

static void __binary_tree_for_each(binary_tree_node_t* node, void* (*fun)(void* a))
{
    if(node != NULL)
    {
        binary_tree_node_t* root = node;
        binary_tree_node_t* left = node->left;
        binary_tree_node_t* right = node->right;
        if(root->data)
            fun(root->data);
        __binary_tree_for_each(left, fun);
        __binary_tree_for_each(right, fun);
    }
}

static void __dealloc_all_node(binary_tree_node_t* node, void (*fun)(void*))
{
    if(node != NULL)
    {
        binary_tree_node_t* root = node;
        binary_tree_node_t* left = node->left;
        binary_tree_node_t* right = node->right;
        if(root->data)
            fun(root->data);
        __dealloc_all_node(left, fun);
        __dealloc_all_node(right, fun);
        allocator_free(root);
    }
}

static inline binary_tree_node_t* binary_tree_node_alloc(void)
{
    binary_tree_node_t* node = allocator_alloc(sizeof(*node));
    if(node == NULL)
    {
        print_err("Cannot allocate memory for new node in binary_tree!\n");
        return NULL;
    }
    node->data = NULL;
    node->right = NULL;
    node->left = NULL;
    return node;
}

binary_tree_node_t* binary_tree_add_child(binary_tree_node_t* parent, void* data)
{
    if(binary_tree_have_child(parent))
    {
        return binary_tree_add_sibling(parent->left, data);
    }
    else
    {
        return binary_tree_add_left(parent, data);
    }
}

binary_tree_node_t* binary_tree_add_sibling(binary_tree_node_t* parent, void* data)
{
    binary_tree_node_t* active = parent;
    while(active->right)
    {
        active = active->right;
    }
    return binary_tree_add_right(active, data);
}

i32 binary_tree_have_child(binary_tree_node_t* parent)
{
    if(parent->left)
    {
        return true;
    }
    return false;
}
