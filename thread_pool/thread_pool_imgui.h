#pragma once

#ifdef THREADPOOL_IMGUI_DEBUG

#include <imgui.h>

namespace multithreads {

inline void ThreadPool::renderDebugWindow(const char* window_name) const {
  ImGui::Begin(window_name);
  
  // Thread statistics
  ImGui::Text("Thread Pool Statistics");
  ImGui::Separator();
  
  size_t active = getActiveThreadCount();
  size_t total = getTotalThreadCount();
  size_t queue_size = getQueueSize();
  
  // Active threads display
  ImGui::Text("Active Threads: %zu / %zu", active, total);
  
  // Progress bar for thread utilization
  float utilization = total > 0 ? static_cast<float>(active) / static_cast<float>(total) : 0.0f;
  ImGui::ProgressBar(utilization, ImVec2(-1.0f, 0.0f));
  
  ImGui::Spacing();
  
  // Queue information
  ImGui::Text("Queue Size: %zu", queue_size);
  
  ImGui::Spacing();
  ImGui::Separator();
  
  // Additional metrics
  ImGui::Text("Utilization: %.1f%%", utilization * 100.0f);
  
  // Color-coded status indicator
  if (active == total && queue_size > 0) {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Status: BUSY (Queue Backlog)");
  } else if (active > 0) {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: ACTIVE");
  } else if (queue_size > 0) {
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Status: IDLE (Tasks Queued)");
  } else {
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Status: IDLE");
  }
  
  ImGui::End();
}

} // namespace multithreads

#endif // THREADPOOL_IMGUI_DEBUG
