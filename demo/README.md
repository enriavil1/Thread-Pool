# ThreadPool ImGui Demo Application

This directory contains demo applications that showcase the ThreadPool library with ImGui debugging support.

## Demo Applications

### 1. `demo_app.cpp` - Interactive Demo
A fully interactive ImGui application where you can:
- Manually add tasks to the thread pool
- Configure task duration ranges
- Enable auto-add mode to continuously add tasks
- View real-time statistics in the debug window
- See the ImGui demo window for reference

**Build and Run:**
```bash
cd demo
mkdir build && cd build
cmake ..
cmake --build .
./demo_app
```

### 2. `demo_focused.cpp` - Monitor Window Only
A focused demo that displays **only** the ThreadPool Monitor debug window created by `renderDebugWindow()` from `thread_pool_imgui.h`. This is used to generate the GIF for the README.

**Features:**
- No window decorations for clean screenshots
- Window size: 420x280 (perfect fit for debug window)
- Saves frames to `/tmp` (automatically cleaned up)
- Runs for 15 seconds with varying workload

**Generate GIF:**
```bash
cd demo
./generate_monitor_gif.sh
```

This script:
1. Runs `demo_focused` with virtual display (Xvfb)
2. Captures 20 frames to `/tmp`
3. Creates animated GIF at `../assets/threadpool_monitor.gif`
4. Cleans up all temporary PNG files
5. Only the final GIF remains

## What the Demo Shows

The ThreadPool Monitor window displays:
- **Active Threads**: Number of threads currently executing tasks (e.g., "8 / 8")
- **Queue Size**: Number of tasks waiting in the queue
- **Tasks Completed**: Cumulative count of finished tasks
- **Utilization**: Percentage bar showing thread usage
- **Status**: Color-coded indicator:
  - 🟢 **ACTIVE** (green): Threads are working
  - 🟡 **IDLE (Tasks Queued)** (yellow): Tasks waiting, no active threads
  - 🟠 **BUSY (Queue Backlog)** (orange): All threads active + queue has tasks
  - ⚪ **IDLE** (gray): No activity

## Requirements

- GLFW3
- OpenGL 3.3+
- ImGui (cloned from official repository)
- C++20 compiler
- CMake 3.10+
- Xvfb (for GIF generation)
- ImageMagick (for GIF creation)

## Building from Scratch

```bash
# Install dependencies
sudo apt-get install libglfw3-dev libgl1-mesa-dev xvfb imagemagick

# Clone ImGui (if not already cloned)
cd /path/to/parent/directory
git clone https://github.com/ocornut/imgui.git

# Build demo
cd Thread-Pool/demo
mkdir build && cd build
cmake ..
cmake --build .

# Run interactive demo
./demo_app

# Or generate GIF
cd ..
./generate_monitor_gif.sh
```

## File Structure

```
demo/
├── demo_app.cpp              # Interactive demo with controls
├── demo_focused.cpp          # Focused demo (monitor window only)
├── demo_screenshot.cpp       # Legacy: full demo with screenshots
├── generate_monitor_gif.sh   # Script to generate GIF for README
├── CMakeLists.txt           # Build configuration
├── stb_image_write.h        # Image writing library
└── README.md                # This file
```

## Notes

- The focused demo saves screenshots to `/tmp` to avoid cluttering the repo
- Only the final GIF (`assets/threadpool_monitor.gif`) is kept in the repository
- The GIF is 60KB and shows 20 frames over 10 seconds
- The demo automatically exits after 15 seconds
