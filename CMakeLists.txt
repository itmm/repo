cmake_minimum_required(VERSION 3.24)
project(repo)

set(CMAKE_CXX_STANDARD 20)

add_executable(repo retriever.cpp retriever.h main.cpp server.cpp server.h selector.cpp selector.h err.h selector-handler.cpp selector-handler.h)
