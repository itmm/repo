cmake_minimum_required(VERSION 3.18.4)
project(repo)

set(CMAKE_CXX_STANDARD 20)

add_executable(repo retriever.cpp retriever.h main.cpp server.cpp server.h selector.cpp selector.h err.h selector-handler.cpp selector-handler.h session.cpp session.h reader.cpp reader.h test-reader.cpp test-reader.h)
