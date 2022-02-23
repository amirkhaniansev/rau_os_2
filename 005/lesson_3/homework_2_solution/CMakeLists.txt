cmake_minimum_required(VERSION 3.5)

project(queue_exp)

file(GLOB queue_sources     "./queue/*.c")
file(GLOB parallel_sources  "./parallel/*.c")
file(GLOB queue_exp_sources "./*.c")

add_library(queue SHARED ${queue_sources})
target_include_directories(queue PRIVATE "./include")

add_library(parallel SHARED ${parallel_sources})
target_include_directories(parallel PRIVATE "./include")

add_executable(queue_exp ${queue_exp_sources})
target_include_directories(queue_exp PRIVATE "./include")
target_link_libraries(queue_exp PRIVATE queue)
target_link_libraries(queue_exp PRIVATE parallel)
target_link_libraries(queue_exp PRIVATE pthread)