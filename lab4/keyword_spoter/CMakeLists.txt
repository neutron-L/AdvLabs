cmake_minimum_required(VERSION 3.23)
project(keyword_spoter C)

set(CMAKE_C_STANDARD 23)

add_executable(keyword_spoter main.c heap.c heap.h queue.c queue.h graph.c graph.h ac.c ac.h matrix.c matrix.h list.c list.h tree.c tree.h tuple.c tuple.h)
