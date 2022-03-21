cmake_minimum_required(VERSION 3.5)

project(threadpool)

file(GLOB source_files "./*.cc")
add_executable(threadpool ${source_files})

target_include_directories(threadpool PRIVATE "./include")
target_link_libraries(threadpool PRIVATE pthread)

