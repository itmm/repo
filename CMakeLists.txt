cmake_minimum_required(VERSION 3.24)
project(repo)

set(CMAKE_CXX_STANDARD 20)

add_executable(repo main.cpp retriever.cpp retriever.h)
