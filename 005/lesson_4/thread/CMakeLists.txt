cmake_minimum_required(VERSION 3.5)

project(reusable_thread)

add_executable(reusable_thread main.cc)

target_link_libraries(reusable_thread PRIVATE pthread)