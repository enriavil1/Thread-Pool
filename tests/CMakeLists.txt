cmake_minimum_required(VERSION 3.10)
project(ThreadPoolTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Include the thread pool library headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

# Test executable
add_executable(thread_pool_tests test_thread_pool.cpp)
target_link_libraries(thread_pool_tests PRIVATE MultiThreadLib)

# Add test
add_test(NAME ThreadPoolTests COMMAND thread_pool_tests)
