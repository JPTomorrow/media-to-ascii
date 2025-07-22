cmake_minimum_required(VERSION 3.31)

set(ProjectTitle media_to_ascii)
project(${ProjectTitle} VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${ProjectTitle} src/main.cpp)

target_link_libraries(${ProjectTitle} ${OpenCV_LIBS})

# add and include all libraries in src folder
file(GLOB_RECURSE ALL_SOURCES
        "src/*.cpp"
        "src/*.c"
        "src/*.h"
        "src/*.hpp"
)
target_sources(${ProjectTitle} PRIVATE ${ALL_SOURCES})
target_include_directories(${ProjectTitle} PRIVATE src/)
