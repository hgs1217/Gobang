cmake_minimum_required(VERSION 3.8)
project(GobangAI)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES ai.cpp)
add_library(GobangAI SHARED ${SOURCE_FILES})