//
// Created by piotr on 06.05.24.
//

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#ifdef LIB_TEST
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <types.h>
#include <cmocka.h>

static inline void* allocator_alloc(size_t size)
{
    return test_malloc(size);
}

static inline void allocator_free(void* ptr)
{
    test_free(ptr);
}

static inline void* allocator_calloc(const size_t nmemb, const size_t size)
{
    return test_calloc(nmemb, size);
}

#else
#include <stdlib.h>

static inline void* allocator_alloc(size_t size)
{
    return malloc(size);
}

static inline void allocator_free(void* ptr)
{
    free(ptr);
}

static inline void* allocator_calloc(const size_t nmemb, const size_t size)
{
    return calloc(nmemb, size);
}
#endif
#endif //ALLOCATOR_H
