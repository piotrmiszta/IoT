set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/server/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/server/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")

add_library(server_lib STATIC    ${HEADER_LIST}
                             ${src})
                                
target_link_libraries(server_lib PUBLIC "lib_common")
target_include_directories(server_lib PUBLIC ${INCLUDE_DIR}/)
add_subdirectory(test)