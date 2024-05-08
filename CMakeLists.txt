cmake_minimum_required(VERSION 3.5)
set(PROJ_NAME "iot")
project(${PROJ_NAME})

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-pthread -Wall -pedantic -g -Werror -lm -O0")

add_executable(server_out server/src/main.c)

enable_testing()
target_link_libraries(server_out PUBLIC "server_lib")
target_link_libraries(server_out PUBLIC "lib_common")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)

add_subdirectory(server)
add_subdirectory(lib)
