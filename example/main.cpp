/*
 * Example application demonstrating ThreadPool with ImGui debugging
 * 
 * To build with ImGui support:
 * 1. Ensure ImGui is available in your project
 * 2. Configure with: cmake -DTHREADPOOL_ENABLE_IMGUI_DEBUG=ON ..
 * 3. Link ImGui to this executable
 * 
 * This example shows:
 * - Basic thread pool usage
 * - How to integrate the debug window into an ImGui application
 * - Real-time monitoring of thread pool statistics
 */

#include <thread_pool/thread_pool.h>
#include <iostream>
#include <chrono>
#include <random>

#ifdef THREADPOOL_IMGUI_DEBUG
#include <imgui.h>
// Include your ImGui backend headers here
// #include <imgui_impl_glfw.h>
// #include <imgui_impl_opengl3.h>
#endif

// Simulate some work
void simulateWork(int task_id, int duration_ms) {
    std::cout << "Task " << task_id << " started (duration: " << duration_ms << "ms)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    std::cout << "Task " << task_id << " completed\n";
}

int main() {
    // Create a thread pool with 4 threads
    multithreads::ThreadPool pool(4);
    
    std::cout << "ThreadPool created with 4 threads\n";
    std::cout << "Adding tasks...\n\n";
    
#ifdef THREADPOOL_IMGUI_DEBUG
    std::cout << "ImGui debugging is ENABLED\n";
    std::cout << "In your ImGui render loop, call: pool.renderDebugWindow();\n\n";
    
    /*
     * Typical ImGui integration in your render loop:
     * 
     * while (!glfwWindowShouldClose(window)) {
     *     // Start ImGui frame
     *     ImGui_ImplOpenGL3_NewFrame();
     *     ImGui_ImplGlfw_NewFrame();
     *     ImGui::NewFrame();
     *     
     *     // Render the thread pool debug window
     *     pool.renderDebugWindow("ThreadPool Monitor");
     *     
     *     // Your other ImGui windows...
     *     
     *     // Render
     *     ImGui::Render();
     *     // ... rendering code ...
     * }
     */
#else
    std::cout << "ImGui debugging is DISABLED\n";
    std::cout << "To enable, build with -DTHREADPOOL_ENABLE_IMGUI_DEBUG=ON\n\n";
#endif
    
    // Add some tasks with varying durations
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 1000);
    
    std::vector<std::future<void>> futures;
    
    for (int i = 0; i < 20; ++i) {
        int duration = dis(gen);
        auto future = pool.addTask(simulateWork, i, duration);
        futures.push_back(std::move(future));
        
        // Add a small delay between task submissions to see queue behavior
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Monitor statistics while tasks are running
    std::cout << "\n=== Monitoring Thread Pool ===\n";
    
    bool all_done = false;
    while (!all_done) {
        std::cout << "Active: " << pool.getActiveThreadCount() 
                  << "/" << pool.getTotalThreadCount()
                  << " | Queue: " << pool.getQueueSize()
                  << " | Completed: " << pool.getTasksCompleted() << "\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        // Check if all futures are ready
        all_done = true;
        for (auto& f : futures) {
            if (f.valid() && f.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                all_done = false;
                break;
            }
        }
    }
    
    // Wait for all tasks to complete
    for (auto& f : futures) {
        if (f.valid()) {
            f.get();
        }
    }
    
    std::cout << "\n=== Final Statistics ===\n";
    std::cout << "Total threads: " << pool.getTotalThreadCount() << "\n";
    std::cout << "Tasks completed: " << pool.getTasksCompleted() << "\n";
    std::cout << "Queue size: " << pool.getQueueSize() << "\n";
    
    return 0;
}
