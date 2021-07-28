cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(moderncpp VERSION 0.1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.11.0
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.9.1
)

FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory("${googletest_SOURCE_DIR}" "${googletest_BINARY_DIR}")
endif()

FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory("${spdlog_SOURCE_DIR}" "${spdlog_BINARY_DIR}")
endif()

add_subdirectory(contrib)
add_subdirectory(examples)
add_subdirectory(exercises)
