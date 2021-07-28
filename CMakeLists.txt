cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(moderncpp VERSION 0.1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(contrib)
add_subdirectory(examples)
add_subdirectory(exercises)
