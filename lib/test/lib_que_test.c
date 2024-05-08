//
// Created by piotr on 06.05.24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "queue.h"

struct test_param
{
    queue_t* que;
    u32* arr;
};

static int initialize(void** state)
{
    queue_t* que = queue_create(NULL);
    u32* arr = test_malloc(sizeof(u32) * 10);
    for(int i = 0; i < 10; i++)
    {
        arr[i] = i;
        queue_push(que, &arr[i]);
    }
    struct test_param* param = test_malloc(sizeof(struct test_param));
    param->que = que;
    param->arr = arr;
    *state = param;
    return 0;
}

static int teardown(void** state)
{
    struct test_param* param = *state;
    queue_t* que = param->que;
    u32* arr = param->arr;
    queue_destroy(que);
    test_free(arr);
    test_free(param);
    return 0;
}

static void test_pop(void** state)
{
    struct test_param* param = *state;
    queue_t* que = param->que;
    for(int i = 0; i < 10; i++)
    {
        u32* data = queue_pop(que);
        assert_int_equal(*data, i);
    }
}

static void test_size(void** state)
{
    struct test_param* param = *state;
    assert_int_equal(queue_size(param->que), 10);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_size, initialize, teardown),
        cmocka_unit_test_setup_teardown(test_pop, initialize, teardown)
};
    return cmocka_run_group_tests_name("queue_success_tests",tests, NULL, NULL);
}