# ImGui Debugging Feature Design

## Overview
Add an optional ImGui debugging window to the ThreadPool library that displays real-time statistics about thread usage and task queue status.

## Architecture

### Build Flag System
- CMake option: `THREADPOOL_ENABLE_IMGUI_DEBUG` (default: OFF)
- When enabled, defines preprocessor macro `THREADPOOL_IMGUI_DEBUG`
- ImGui dependency is only required when the flag is enabled

### Implementation Strategy

#### 1. Thread Activity Tracking
- Add atomic counters for:
  - `active_threads_` - number of threads currently executing tasks
  - `total_threads_` - total number of threads in the pool
  - `tasks_completed_` - cumulative count of completed tasks
  - `tasks_queued_` - current number of tasks in queue

#### 2. ImGui Integration
- Create a new header: `thread_pool/thread_pool_imgui.h`
- Provide a `renderDebugWindow()` method that can be called from the application's ImGui render loop
- The method will display:
  - Active threads vs total threads
  - Current queue size
  - Tasks completed counter
  - Thread utilization percentage

#### 3. Conditional Compilation
- Use `#ifdef THREADPOOL_IMGUI_DEBUG` to wrap ImGui-specific code
- When disabled, the tracking overhead is minimal (just atomic counters)
- The debug window method is only available when the flag is enabled

### File Structure
```
thread_pool/
  ├── thread_pool.h           # Core implementation with optional tracking
  └── thread_pool_imgui.h     # ImGui rendering code (conditionally compiled)
```

### CMake Changes
```cmake
option(THREADPOOL_ENABLE_IMGUI_DEBUG "Enable ImGui debugging window" OFF)

if(THREADPOOL_ENABLE_IMGUI_DEBUG)
    target_compile_definitions(${PROJECT_NAME} INTERFACE THREADPOOL_IMGUI_DEBUG)
    # User must provide ImGui in their project
endif()
```

### Usage Example
```cpp
// Without debug flag - normal usage
multithreads::ThreadPool pool(4);
pool.addTask([]{ /* work */ });

// With debug flag enabled
multithreads::ThreadPool pool(4);
pool.addTask([]{ /* work */ });

// In ImGui render loop:
pool.renderDebugWindow();
```

## Benefits
- Zero overhead when disabled (header-only remains header-only)
- Optional dependency on ImGui
- Real-time visibility into thread pool behavior
- Maintains library portability
