# Add Optional ImGui Debugging Window Feature

## Overview
This PR adds an optional ImGui debugging window to the ThreadPool library that provides real-time visibility into thread pool statistics and behavior. The feature is controlled via a CMake build flag and maintains the library's header-only structure.

## Features Added

### 1. Thread Activity Tracking
- **Active threads counter**: Tracks how many threads are currently executing tasks
- **Tasks completed counter**: Cumulative count of all completed tasks
- **Queue size monitoring**: Real-time view of pending tasks in the queue
- **Minimal overhead**: Uses atomic counters that have negligible performance impact even when ImGui is disabled

### 2. ImGui Debug Window
- **Real-time statistics display**: Shows active threads, queue size, and tasks completed
- **Visual utilization bar**: Progress bar showing thread pool utilization percentage
- **Color-coded status indicators**:
  - 🟢 **ACTIVE**: Threads are working
  - 🟡 **IDLE (Tasks Queued)**: No active threads but tasks are waiting
  - 🟠 **BUSY (Queue Backlog)**: All threads active with tasks still queued
  - ⚪ **IDLE**: No activity
- **Customizable window name**: Pass a custom title to `renderDebugWindow()`

### 3. Build Flag Control
- **CMake option**: `THREADPOOL_ENABLE_IMGUI_DEBUG` (default: OFF)
- **Conditional compilation**: ImGui code is only compiled when the flag is enabled
- **Zero dependency when disabled**: The library remains header-only with no ImGui dependency

### 4. Example Application
- Comprehensive example demonstrating thread pool usage
- Shows how to integrate the debug window into an ImGui application
- Includes monitoring code that works with or without ImGui enabled

## Files Changed

### Modified Files
- **`thread_pool/thread_pool.h`**: Added atomic counters for tracking and getter methods
- **`CMakeLists.txt`**: Added `THREADPOOL_ENABLE_IMGUI_DEBUG` option
- **`README.md`**: Comprehensive documentation with usage examples

### New Files
- **`thread_pool/thread_pool_imgui.h`**: ImGui rendering implementation
- **`example/main.cpp`**: Example application demonstrating the feature
- **`example/CMakeLists.txt`**: Build configuration for the example
- **`DESIGN.md`**: Design documentation and architecture overview

## Usage

### Building Without ImGui (Default)
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Building With ImGui Debug Support
```bash
mkdir build && cd build
cmake -DTHREADPOOL_ENABLE_IMGUI_DEBUG=ON ..
cmake --build .
```

### Using the Debug Window
```cpp
#include <thread_pool/thread_pool.h>

multithreads::ThreadPool pool(8);

// In your ImGui render loop:
pool.renderDebugWindow("My ThreadPool Debug");
```

## Benefits

1. **Optional Feature**: Zero impact when disabled, maintains library simplicity
2. **Real-time Monitoring**: Helps debug performance issues and understand thread behavior
3. **Easy Integration**: Single method call to render the debug window
4. **Maintains Library Structure**: Still header-only, still portable
5. **Minimal Overhead**: Atomic counters have negligible performance cost

## Testing

- ✅ Builds successfully without ImGui flag (default)
- ✅ Builds successfully with ImGui flag enabled
- ✅ Example application compiles and runs correctly
- ✅ Thread tracking works as expected (verified with example output)
- ✅ Library remains header-only

## Backward Compatibility

This PR is **100% backward compatible**:
- Default behavior unchanged (ImGui disabled by default)
- No breaking changes to existing API
- Existing code continues to work without modification
- New features are opt-in via build flag

## Screenshots

The example application output demonstrates the tracking working correctly:
```
ThreadPool created with 4 threads
Adding tasks...
ImGui debugging is DISABLED

=== Monitoring Thread Pool ===
Active: 4/4 | Queue: 12 | Completed: 4
Active: 4/4 | Queue: 11 | Completed: 5
Active: 4/4 | Queue: 6 | Completed: 10
...
=== Final Statistics ===
Total threads: 4
Tasks completed: 20
Queue size: 0
```

## Future Enhancements

Potential future additions (not included in this PR):
- Per-thread statistics (individual thread utilization)
- Task execution time histograms
- Queue wait time metrics
- Thread pool performance graphs over time

## Checklist

- [x] Code follows project style
- [x] Documentation updated
- [x] Example application included
- [x] Backward compatible
- [x] Tested with and without ImGui flag
- [x] Header-only structure maintained
