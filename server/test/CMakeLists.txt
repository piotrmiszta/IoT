set(TEST_DIR "${CMAKE_SOURCE_DIR}/server/test/")

list(APPEND TEST_LIBS "cmocka")
list(APPEND TEST_LIBS "lib_common")
list(APPEND TEST_LIBS "server_lib")


set(CMAKE_C_STANDARD 17)
list(APPEND TEST_DIRS "${CMOCKA_INCLUDE_DIRS}")
list(APPEND TEST_DIRS "${CMAKE_SOURCE_DIR}/src")
include_directories(${CMAKE_SOURCE_DIR}/src)

add_cmocka_test(server_lib_template
  SOURCES ${TEST_DIR}/simple_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(server_lib_template)
target_include_directories(server_lib_template PUBLIC "${TEST_DIRS}")