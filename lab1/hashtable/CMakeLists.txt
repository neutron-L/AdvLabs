cmake_minimum_required(VERSION 3.21)
project(hashtable)

set(CMAKE_CXX_STANDARD 14)

add_executable(hashtable_search main.cpp item.cpp item.h list.cpp list.h hashtable.cpp hashtable.h hash.h hash.cpp avltree.cpp avltree.h rbtree.cpp rbtree.h dsl.cpp dsl.h)
