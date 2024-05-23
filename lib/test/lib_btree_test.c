//
// Created by piotr on 06.05.24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "btree.h"
#include "allocator.h"


int initialize(void** state)
{
    btree_t* tree;
    tree = btree_create(allocator_free);
    btree_node_t* node = btree_get_root(tree);
    for(int i = 0; i < 10; i++)
    {
        int* r = test_malloc(sizeof(int));
        int* l = test_malloc(sizeof(int));
        *r = i;
        *l = i;
        btree_add_left(node, l);
        btree_add_right(node, r);
        node = btree_get_left(node);
    }
    *state = tree;
    return 0;
}

int teardown(void** state)
{
    btree_t* tree= *state;
    btree_destroy(tree);
    return 0;
}

static inline void* btree_for_each_test_fun(void* a)
{
    int* val = a;
    *val += 1;
    return val;
}

void btree_for_each_test(void** state)
{
    btree_t* tree= *state;
    btree_for_each(tree, btree_for_each_test_fun);
    btree_node_t* node = btree_get_left(btree_get_root(tree));
    for(int i = 0; i < 10; i++)
    {
        int* k = btree_get_data(node);
        node = btree_get_left(node);
        assert_int_equal(*k, i + 1);
    }
}

void btree_get_test(void** state)
{
    btree_t* tree= *state;
    btree_node_t* node = btree_get_left(btree_get_root(tree));
    for(int i = 0; i < 10; i++)
    {
        int* k = btree_get_data(node);
        node = btree_get_left(node);
        assert_int_equal(*k, i);
    }
}

static inline i32 search(const void* a, const void* b)
{
    const int aa = *(int*)a;
    const int bb = *(int*)b;
    return aa==bb;
}

void btree_search_test(void** state)
{
    btree_t* tree= *state;
    int k = 4;
    const btree_node_t* t = btree_search(tree, &k, search);
   // assert_ptr_not_equal(t, NULL);
    int* ptr = btree_get_data(t);
    assert_int_equal(*(int*)ptr, 4);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(btree_get_test, initialize, teardown),
        cmocka_unit_test_setup_teardown(btree_for_each_test, initialize, teardown),
        cmocka_unit_test_setup_teardown(btree_search_test, initialize, teardown),
    };
    return cmocka_run_group_tests_name("btree_success_tests",tests, NULL, NULL);
}