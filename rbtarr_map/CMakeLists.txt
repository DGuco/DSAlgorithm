cmake_minimum_required(VERSION 3.17)
project(rbtarr_map)

set(CMAKE_CXX_STANDARD 11)

add_executable(rbtarr_map main.cpp hash_helper.h rbthash_map.h rb_tree.h hash_function.h)

target_link_libraries(rbtarr_map pthread)
