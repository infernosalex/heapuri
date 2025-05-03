cmake_minimum_required(VERSION 3.10)

project(heapuri LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# directory for headers
include_directories(include)

# source files
set(SOURCES
    src/avl.cpp
    src/scapegoat.cpp
    src/main.cpp
)

# create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
