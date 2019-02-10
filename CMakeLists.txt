cmake_minimum_required(VERSION 2.8.4)

project(tshell)

add_subdirectory(googletest)

set(CMAKE_CXX_STANDARD 11)

add_executable(tshell 
    src/main.cpp
    src/parser.cpp
)

add_executable(test 
    tests/test.cpp 
    src/parser.cpp
)

target_link_libraries(test gtest)
