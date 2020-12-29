cmake_minimum_required(VERSION 3.17)
project(HashTable)

set(CMAKE_CXX_STANDARD 14)

add_executable(HashTable word_counter_test.cpp WordCounter.cpp WordCounter.h English.cpp English.h)