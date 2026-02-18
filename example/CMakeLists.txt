cmake_minimum_required(VERSION 3.10)
project(ThreadPoolExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the thread pool library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/thread_pool)

# Create the example executable
add_executable(thread_pool_example main.cpp)
target_link_libraries(thread_pool_example PRIVATE MultiThreadLib)

# If ImGui debugging is enabled, you would link ImGui here
# Example (adjust paths based on your ImGui setup):
# if(THREADPOOL_ENABLE_IMGUI_DEBUG)
#     find_package(imgui REQUIRED)
#     target_link_libraries(thread_pool_example PRIVATE imgui::imgui)
# endif()
