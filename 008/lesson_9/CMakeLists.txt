cmake_minimum_required(VERSION 3.5)

project(ipc)

add_executable(server "server.cc")
target_link_libraries(server PRIVATE pthread)
target_link_libraries(server PRIVATE rt)

add_executable(client "client.cc")
target_link_libraries(client PRIVATE pthread)
target_link_libraries(client PRIVATE rt)