cmake_minimum_required(VERSION 3.10)
project(
  MultiThread
  VERSION 1.0.0
  LANGUAGES CXX)

set(PROJECT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.h)
add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})
