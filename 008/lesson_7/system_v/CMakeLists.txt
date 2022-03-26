cmake_minimum_required(VERSION 3.5)

project(message_queue)

add_executable(sender "sender.c")
target_link_libraries(sender PRIVATE pthread)

add_executable(receiver "receiver.c")
target_link_libraries(receiver PRIVATE pthread)