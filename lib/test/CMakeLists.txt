set(TEST_DIR "${CMAKE_SOURCE_DIR}/lib/test/")

list(APPEND TEST_LIBS "cmocka")
list(APPEND TEST_LIBS "lib_common_test")

set(CMAKE_C_STANDARD 17)
list(APPEND TEST_DIRS "${CMOCKA_INCLUDE_DIRS}")
list(APPEND TEST_DIRS "${CMAKE_SOURCE_DIR}/src")
include_directories(${CMAKE_SOURCE_DIR}/src)

add_cmocka_test(lib_list_test
  SOURCES ${TEST_DIR}/lib_list_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(lib_list_test)
target_include_directories(lib_list_test PUBLIC "${TEST_DIRS}")

add_cmocka_test(lib_queue_test
  SOURCES ${TEST_DIR}/lib_que_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(lib_queue_test)
target_include_directories(lib_queue_test PUBLIC "${TEST_DIRS}")