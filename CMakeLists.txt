cmake_minimum_required(VERSION 3.26)
project(sw_battle)
set(CMAKE_CXX_STANDARD 17)

#-------------------------------------------------------------------
# Определение опций сборки
#-------------------------------------------------------------------
option(BUILD_TESTS "Build tests. Google test framework is used for this!" OFF)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC src/)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()