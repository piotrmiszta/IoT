//
// Created by piotr on 06.05.24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "list.h"

struct test_param
{
    list_t* list;
    u32* arr;
};

static int initialize(void** state)
{
    list_t* list = list_create(NULL);
    u32* arr = test_malloc(sizeof(u32) * 10);
    for(int i = 0; i < 10; i++)
    {
        arr[i] = i;
        list_add_tail(list, &arr[i]);
    }
    struct test_param* param = test_malloc(sizeof(struct test_param));
    param->list = list;
    param->arr = arr;
    *state = param;
    return 0;
}

static int teardown(void** state)
{
    struct test_param* param = *state;
    list_t* list = param->list;
    u32* arr = param->arr;
    list_destroy(list);
    test_free(arr);
    test_free(param);
    return 0;
}

static void test_get_index(void ** state) {
    struct test_param* param = *state;
    for(int i = 0; i < 10; i++)
    {
        u32* ptr = list_get_index(param->list, 0);
        assert_int_equal(*ptr, i);
    }
}

static void test_get_head(void ** state) {
    struct test_param* param = *state;
    for(int i = 0; i < 10; i++)
    {
        u32* ptr = list_get_head(param->list);
        assert_int_equal(*ptr, i);
    }
}

static void test_get_tail(void ** state) {
    struct test_param* param = *state;
    for(int i = 0; i < 10; i++)
    {
        u32* ptr = list_get_tail(param->list);
        assert_int_equal(*ptr, 9 - i);
    }
}

static void test_size(void** state)
{
    struct test_param* param = *state;
    for(int i = 0; i < 10; i++)
    {
        list_get_tail(param->list);
        assert_int_equal(list_size(param->list), 9 - i);
    }
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_tail, initialize, teardown),
        cmocka_unit_test_setup_teardown(test_get_head, initialize, teardown),
        cmocka_unit_test_setup_teardown(test_get_index, initialize, teardown),
        cmocka_unit_test_setup_teardown(test_size, initialize, teardown)
};
    return cmocka_run_group_tests_name("list_success_tests",tests, NULL, NULL);
}