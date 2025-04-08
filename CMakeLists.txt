cmake_minimum_required(VERSION 3.23.2)
project(socket)

set(CMAKE_CXX_STANDARD 11)

include_directories(./)

file(GLOB_RECURSE SOURCES "utility/*.cpp" "socket/*.cpp")

add_executable(server ${SOURCES} server.cpp)
add_executable(server_v2 ${SOURCES} server_v2.cpp)
add_executable(client ${SOURCES} client.cpp)
