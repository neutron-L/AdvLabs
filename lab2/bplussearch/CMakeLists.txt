cmake_minimum_required(VERSION 3.23)
project(bplussearch C)

set(CMAKE_C_STANDARD 23)

add_executable(bplussearch main.c bplustree.h bplustree.c)
