//
// Created by piotr on 30.04.24.
//

#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stdbool.h>
#include "assert.h"
#include "error.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef int32_t err_t;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif //TYPES_H
