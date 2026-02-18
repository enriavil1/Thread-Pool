// Demo application showing ThreadPool with ImGui debug window
#include <thread_pool/thread_pool.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>

// Simulate work with varying durations
void simulateWork(int task_id, int duration_ms) {
    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // GL 3.3 + GLSL 330
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ThreadPool ImGui Debug Demo", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create thread pool
    multithreads::ThreadPool pool(8);

    // Demo state
    int task_duration_min = 100;
    int task_duration_max = 2000;
    int tasks_to_add = 5;
    int next_task_id = 0;
    bool auto_add_tasks = false;
    float auto_add_interval = 1.0f;
    float time_since_last_add = 0.0f;

    std::random_device rd;
    std::mt19937 gen(rd());

    // Main loop
    auto last_frame_time = std::chrono::steady_clock::now();
    
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        auto current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_frame_time).count();
        last_frame_time = current_time;

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Auto-add tasks if enabled
        if (auto_add_tasks) {
            time_since_last_add += delta_time;
            if (time_since_last_add >= auto_add_interval) {
                std::uniform_int_distribution<> dis(task_duration_min, task_duration_max);
                int duration = dis(gen);
                pool.addTask(simulateWork, next_task_id++, duration);
                time_since_last_add = 0.0f;
            }
        }

        // Main control window
        ImGui::Begin("ThreadPool Demo Controls");
        
        ImGui::Text("Welcome to the ThreadPool ImGui Debug Demo!");
        ImGui::Separator();
        
        ImGui::Text("Thread Pool Configuration:");
        ImGui::Text("  Total Threads: 8");
        ImGui::Spacing();
        
        ImGui::Text("Task Settings:");
        ImGui::SliderInt("Min Duration (ms)", &task_duration_min, 50, 1000);
        ImGui::SliderInt("Max Duration (ms)", &task_duration_max, task_duration_min, 5000);
        ImGui::SliderInt("Tasks to Add", &tasks_to_add, 1, 50);
        
        if (ImGui::Button("Add Tasks Manually", ImVec2(200, 30))) {
            std::uniform_int_distribution<> dis(task_duration_min, task_duration_max);
            for (int i = 0; i < tasks_to_add; ++i) {
                int duration = dis(gen);
                pool.addTask(simulateWork, next_task_id++, duration);
            }
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::Text("Auto-Add Tasks:");
        ImGui::Checkbox("Enable Auto-Add", &auto_add_tasks);
        if (auto_add_tasks) {
            ImGui::SliderFloat("Interval (seconds)", &auto_add_interval, 0.1f, 5.0f);
            ImGui::ProgressBar(time_since_last_add / auto_add_interval, ImVec2(-1, 0));
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::Text("Statistics:");
        ImGui::Text("  Total Tasks Created: %d", next_task_id);
        ImGui::Text("  Active Threads: %zu / %zu", pool.getActiveThreadCount(), pool.getTotalThreadCount());
        ImGui::Text("  Queue Size: %zu", pool.getQueueSize());
        ImGui::Text("  Tasks Completed: %zu", pool.getTasksCompleted());
        
        ImGui::End();

        // Render the ThreadPool debug window
        pool.renderDebugWindow("ThreadPool Monitor");

        // Show ImGui demo window for reference
        ImGui::ShowDemoWindow();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
