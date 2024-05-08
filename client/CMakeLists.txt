set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/client/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/client/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")

add_library(client_lib STATIC   ${HEADER_LIST}
                                ${src})

target_link_libraries(client_lib PUBLIC "lib_common")
target_include_directories(client_lib PUBLIC ${INCLUDE_DIR}/)
add_subdirectory(test)