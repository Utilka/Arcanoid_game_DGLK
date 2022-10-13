cmake_minimum_required(VERSION 3.23)
project(Arcanoid_CMake)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

SET(_WINDOWS ON)

link_directories(framework ${PROJECT_SOURCE_DIR})

add_executable(Arcanoid_CMake
        Framework.h
        GameObject.h
        GameObject.cpp
        game.cpp)

target_link_libraries(Arcanoid_CMake FrameworkRelease_x64)
set_target_properties(Arcanoid_CMake PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})