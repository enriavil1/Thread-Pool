// Focused demo showing only the ThreadPool Monitor window
#include <thread_pool/thread_pool.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <chrono>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void simulateWork(int task_id, int duration_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
}

void saveScreenshot(GLFWwindow* window, const char* filename) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    std::vector<unsigned char> pixels(width * height * 3);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    
    // Flip vertically
    std::vector<unsigned char> flipped(width * height * 3);
    for (int y = 0; y < height; y++) {
        memcpy(&flipped[y * width * 3], &pixels[(height - 1 - y) * width * 3], width * 3);
    }
    
    stbi_write_png(filename, width, height, 3, flipped.data(), width * 3);
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Remove window decorations

    // Smaller window to fit just the debug window
    GLFWwindow* window = glfwCreateWindow(420, 280, "ThreadPool Monitor", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create thread pool
    multithreads::ThreadPool pool(8);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(200, 1500);

    int next_task_id = 0;
    int frame_count = 0;
    int screenshot_count = 0;
    auto start_time = std::chrono::steady_clock::now();

    // Auto-add initial burst of tasks
    for (int i = 0; i < 20; ++i) {
        pool.addTask(simulateWork, next_task_id++, dis(gen));
    }

    while (!glfwWindowShouldClose(window)) {
        auto current_time = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(current_time - start_time).count();
        
        // Run for 15 seconds
        if (elapsed > 15.0f) {
            break;
        }

        glfwPollEvents();

        // Add tasks periodically
        if (frame_count % 60 == 0 && next_task_id < 100) {
            for (int i = 0; i < 3; ++i) {
                pool.addTask(simulateWork, next_task_id++, dis(gen));
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Only show the ThreadPool Monitor window - fill entire window
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(420, 280), ImGuiCond_Always);
        pool.renderDebugWindow("ThreadPool Monitor");

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Save screenshots at intervals to /tmp
        if (frame_count % 30 == 0 && screenshot_count < 20) {
            char filename[256];
            snprintf(filename, sizeof(filename), "/tmp/threadpool_frame_%03d.png", screenshot_count);
            saveScreenshot(window, filename);
            screenshot_count++;
        }

        glfwSwapBuffers(window);
        frame_count++;
    }

    std::cout << "Demo complete. Saved " << screenshot_count << " screenshots." << std::endl;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
