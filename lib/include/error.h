//
// Created by piotr on 06.05.24.
//

#ifndef ERROR_H
#define ERROR_H
#include <stdio.h>
#include <assert.h>
/**
 * @brief macro assert_debug() is used for debugging code
 *        check things that programmer should be aware of
 *        if LIB_DEBUG defs is turned off assertion provides by
 *        assert_debug() is disabled to meet better performance
 *
 */
#ifdef LIB_DEBUG
#define assert_debug(cond)      assert(cond)
#else
#define assert_debug(cond)
#endif

//TODO: stub of error printing
#define print_err(...)      fprintf(stderr, __VA_ARGS__)

#endif //ERROR_H
