//
// Created by piotr on 06.05.24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "binary_tree.h"
#include "allocator.h"


int initialize(void** state)
{
    binary_tree_t* tree;
    tree = binary_tree_create(allocator_free);
    binary_tree_node_t* node = binary_tree_get_root(tree);
    for(int i = 0; i < 10; i++)
    {
        int* r = test_malloc(sizeof(int));
        int* l = test_malloc(sizeof(int));
        *r = i;
        *l = i;
        binary_tree_add_left(node, l);
        binary_tree_add_right(node, r);
        node = binary_tree_get_left(node);
    }
    *state = tree;
    return 0;
}

int teardown(void** state)
{
    binary_tree_t* tree= *state;
    binary_tree_destroy(tree);
    return 0;
}

static inline void* binary_tree_for_each_test_fun(void* a)
{
    int* val = a;
    *val += 1;
    return val;
}

void binary_tree_for_each_test(void** state)
{
    binary_tree_t* tree= *state;
    binary_tree_for_each(tree, binary_tree_for_each_test_fun);
    binary_tree_node_t* node = binary_tree_get_left(binary_tree_get_root(tree));
    for(int i = 0; i < 10; i++)
    {
        int* k = binary_tree_get_data(node);
        node = binary_tree_get_left(node);
        assert_int_equal(*k, i + 1);
    }
}

void binary_tree_get_test(void** state)
{
    binary_tree_t* tree= *state;
    binary_tree_node_t* node = binary_tree_get_left(binary_tree_get_root(tree));
    for(int i = 0; i < 10; i++)
    {
        int* k = binary_tree_get_data(node);
        node = binary_tree_get_left(node);
        assert_int_equal(*k, i);
    }
}

static inline i32 search(const void* a, const void* b)
{
    const int aa = *(int*)a;
    const int bb = *(int*)b;
    return aa==bb;
}

void binary_tree_search_test(void** state)
{
    binary_tree_t* tree= *state;
    int k = 4;
    const binary_tree_node_t* t = binary_tree_search(tree, &k, search);
   // assert_ptr_not_equal(t, NULL);
    int* ptr = binary_tree_get_data(t);
    assert_int_equal(*(int*)ptr, 4);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(binary_tree_get_test, initialize, teardown),
        cmocka_unit_test_setup_teardown(binary_tree_for_each_test, initialize, teardown),
        cmocka_unit_test_setup_teardown(binary_tree_search_test, initialize, teardown),
    };
    return cmocka_run_group_tests_name("binary_tree_success_tests",tests, NULL, NULL);
}