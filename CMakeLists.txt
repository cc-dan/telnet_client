project(telnet_client)
cmake_minimum_required(VERSION 3.2)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/bin)

include_directories(.)
include_directories(../libsocket)
include_directories(lib/libtelnet)

add_library(socket SHARED IMPORTED)
set_property(TARGET socket PROPERTY
        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/../libsocket/build/lib/libsocket.so)

add_library(libtelnet STATIC IMPORTED)
set_property(TARGET libtelnet PROPERTY
        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/libtelnet/build/lib/liblibtelnet.a)

add_library(telnet_client SHARED telnet_client.cpp)
target_link_libraries(telnet_client PRIVATE socket libtelnet)

add_executable(connection test/connection.cpp) 
target_link_libraries(connection PRIVATE telnet_client)
