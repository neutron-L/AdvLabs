cmake_minimum_required(VERSION 3.23)
project(bp_search C)

set(CMAKE_C_STANDARD 23)

add_executable(bp_search main.c bf.h bf.c)
