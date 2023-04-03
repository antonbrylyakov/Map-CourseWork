cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(Homework)
add_executable(homework main.cpp "thread_pool.cpp" "thread_pool.h" "safe_queue.hpp" )