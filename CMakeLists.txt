cmake_minimum_required(VERSION 3.10)
project(
  MultiThreadLib
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool/thread_pool.h)

add_library(${PROJECT_NAME} INTERFACE ${PROJECT_FILES})
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
