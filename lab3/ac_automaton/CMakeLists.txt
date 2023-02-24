cmake_minimum_required(VERSION 3.23)
project(ac_automaton)

set(CMAKE_C_STANDARD 23)

add_executable(ac_automaton main.c ac.c ac.h queue.h queue.c)
