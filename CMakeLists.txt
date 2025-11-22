cmake_minimum_required(VERSION 3.10)
project(
  MultiThread
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool/thread_pool.h)
add_library(MultiThreadLib STATIC ${PROJECT_FILES})
