cmake_minimum_required(VERSION 3.10)
project(ThreadPoolExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL and GLFW for ImGui demo
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ImGui source files
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add the library with ImGui debug enabled
set(THREADPOOL_ENABLE_IMGUI_DEBUG ON CACHE BOOL "Enable ImGui debugging" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/thread_pool)

# Create focused demo executable (only debug window)
add_executable(demo_focused demo_focused.cpp ${IMGUI_SOURCES})

target_include_directories(demo_focused PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_focused PRIVATE
    MultiThreadLib
    OpenGL::GL
    glfw
)
