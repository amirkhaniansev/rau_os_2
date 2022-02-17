cmake_minimum_required(VERSION 3.5)

project(cond_var)

add_executable(cond_var main.c)

target_link_libraries(cond_var pthread)