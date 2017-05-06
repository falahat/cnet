cmake_minimum_required(VERSION 3.7)
project(CNet)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp)
add_subdirectory(src)
add_subdirectory(lib)

add_executable(CNetRun ${SOURCE_FILES})
target_link_libraries(CNetRun src)
target_link_libraries(CNetRun lib)
