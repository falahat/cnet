cmake_minimum_required(VERSION 3.7)
project(CNet)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp)
set(TEST_FILES tests/TestMatrix.cpp tests/TestMatrix.h tests/TestActivationFunction.h tests/TestLayer.h)
add_subdirectory(src)
add_subdirectory(lib)
#add_subdirectory(tests)

add_executable(CNetRun ${SOURCE_FILES})
add_executable(CNetTests ${TEST_FILES})

target_link_libraries(CNetRun CNet)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

target_link_libraries(CNetTests CNet)
target_link_libraries(CNetTests gtest)

