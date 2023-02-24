cmake_minimum_required(VERSION 3.23)
project(mtrie C)

set(CMAKE_C_STANDARD 23)

add_executable(mtrie main.c trie.h trie.c)
