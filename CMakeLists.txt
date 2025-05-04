cmake_minimum_required(VERSION 3.10)

project(heapuri LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# download and configure Google Benchmark
include(FetchContent)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1  # Use a specific version tag
)
# disable benchmark tests and install
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install" FORCE)
FetchContent_MakeAvailable(benchmark)

# directory for headers
include_directories(include)

# main source files
set(SOURCES
    src/avl.cpp
    src/scapegoat.cpp
    src/main.cpp
)

# create the main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# create the benchmark executable
set(BENCHMARK_SOURCES
    src/avl.cpp
    src/scapegoat.cpp
    src/benchmark.cpp
)

add_executable(benchmarks ${BENCHMARK_SOURCES})
target_link_libraries(benchmarks benchmark::benchmark)
