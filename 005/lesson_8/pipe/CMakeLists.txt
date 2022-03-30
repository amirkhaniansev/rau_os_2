cmake_minimum_required(VERSION 3.5)

project(pipe)

add_executable(pipe "pipe.c")
target_link_libraries(pipe PRIVATE pthread)
target_link_libraries(pipe PRIVATE rt)