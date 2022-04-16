cmake_minimum_required(VERSION 3.19)
project(rbtarr_map)

set(CMAKE_CXX_STANDARD 11)

add_executable(rbtarr_map main.cpp hash_helper.h rbthash_map.h rb_tree.h hash_function.h)