cmake_minimum_required(VERSION 3.5)

project(message_queue)

add_executable(writer "writer.c")
target_link_libraries(writer PRIVATE pthread)
target_link_libraries(writer PRIVATE rt)

add_executable(reader "reader.c")
target_link_libraries(reader PRIVATE pthread)
target_link_libraries(reader PRIVATE rt)