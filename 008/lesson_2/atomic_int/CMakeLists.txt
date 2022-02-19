cmake_minimum_required(VERSION 3.5)

project(atomic_int)

add_executable(atomic_int main.c atomic_int.c)

target_include_directories(atomic_int PRIVATE "./include")
target_link_libraries(atomic_int pthread)
