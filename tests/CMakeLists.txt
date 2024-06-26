cmake_minimum_required(VERSION 3.26)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 17)
message("Added tests!")
file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
file(GLOB_RECURSE MAP_SOURCES ../src/Map/*.cpp)
file(GLOB_RECURSE UNITS_SOURCES ../src/Units/*.cpp)
file(GLOB_RECURSE GMNGR_SOURCES ../src/GameManager/*.cpp)

#----------------------------------------------------------------------------------------------------------------------
# testing framework
#----------------------------------------------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
    googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # do not override parent project's runtime settings on Windows
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)
#----------------------------------------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCES} ${MAP_SOURCES} ${UNITS_SOURCES} ${GMNGR_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC src/)

target_link_libraries(${PROJECT_NAME}
        PRIVATE gtest_main gtest)
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME} DISCOVERY_MODE PRE_TEST)