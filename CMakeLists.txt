cmake_minimum_required(VERSION 3.10)
project(
  MultiThreadLib
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to enable ImGui debugging window
option(THREADPOOL_ENABLE_IMGUI_DEBUG "Enable ImGui debugging window for ThreadPool" OFF)

# Option to build tests
option(THREADPOOL_BUILD_TESTS "Build ThreadPool tests" ON)

set(PROJECT_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool/thread_pool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool/thread_pool_imgui.h)

add_library(${PROJECT_NAME} INTERFACE ${PROJECT_FILES})
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable ImGui debug features if requested
if(THREADPOOL_ENABLE_IMGUI_DEBUG)
    message(STATUS "ThreadPool ImGui debugging enabled")
    target_compile_definitions(${PROJECT_NAME} INTERFACE THREADPOOL_IMGUI_DEBUG)
    
    # Note: ImGui must be provided by the consuming project
    # The user should link ImGui to their executable/library
endif()

# Add tests if enabled (only when building as main project)
if(THREADPOOL_BUILD_TESTS AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    enable_testing()
    add_subdirectory(tests)
endif()
