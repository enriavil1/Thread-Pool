# ThreadPool ImGui Demo Application

This directory contains a fully functional demo application that showcases the ThreadPool library with ImGui debugging support.

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

### 2. `demo_screenshot.cpp` - Automated Demo
An automated version that runs for 15 seconds and captures screenshots at intervals. Used to generate the animated GIF for documentation.

**Build and Run:**
```bash
cd demo/build
./demo_screenshot
```

## Demo Features

The demo applications demonstrate:

- **Thread Pool with 8 Threads**: Shows realistic multi-threaded behavior
- **Dynamic Task Addition**: Tasks are added with varying durations (200-1500ms)
- **Real-Time Monitoring**: The debug window updates in real-time showing:
  - Active threads (4/8, 8/8, etc.)
  - Current queue size
  - Tasks completed counter
  - Thread utilization percentage with progress bar
  - Color-coded status indicators
- **ImGui Integration**: Full ImGui context with GLFW/OpenGL3 backend

## Visual Assets

- **`threadpool_demo.gif`**: Animated GIF showing the debug window in action (20 frames, 50ms delay)
- **`screenshot_*.png`**: Individual frames captured during demo execution
- **`assets/demo_screenshot.png`**: Representative screenshot used in documentation

## Requirements

- GLFW3
- OpenGL 3.3+
- ImGui (cloned from official repository)
- C++20 compiler
- CMake 3.10+

## How It Works

The demo creates a ThreadPool with 8 worker threads and continuously adds tasks with random durations. The ImGui debug window (`renderDebugWindow()`) is called every frame to display real-time statistics.

The debug window shows:
1. **Active Threads**: Number of threads currently executing tasks
2. **Queue Size**: Number of tasks waiting in the queue
3. **Tasks Completed**: Cumulative count of finished tasks
4. **Utilization**: Percentage of threads currently active
5. **Status**: Color-coded indicator (IDLE, ACTIVE, BUSY)

## Building from Scratch

If you want to build the demo from scratch:

```bash
# Install dependencies
sudo apt-get install libglfw3-dev libgl1-mesa-dev

# Clone ImGui
cd /path/to/parent/directory
git clone https://github.com/ocornut/imgui.git

# Build demo
cd Thread-Pool/demo
mkdir build && cd build
cmake ..
cmake --build .

# Run
./demo_app
```

## Notes

- The demo uses Xvfb (virtual framebuffer) for headless screenshot capture
- ImageMagick is used to create the animated GIF from screenshots
- The demo automatically exits after 15 seconds (screenshot version)
- The interactive version runs until you close the window
