set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")

# LIBRARY FOR PROJECT
add_library(lib_common STATIC    ${HEADER_LIST}
                             ${src})

target_include_directories(lib_common PUBLIC ${INCLUDE_DIR}/)

#LIBRARY FOR TESTING
add_library(lib_common_test STATIC    ${HEADER_LIST}
                             ${src})

target_include_directories(lib_common_test PUBLIC ${INCLUDE_DIR}/)
target_compile_options(lib_common_test PUBLIC -O -g -DLIB_TEST)
add_subdirectory(test)
