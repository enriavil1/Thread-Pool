#pragma once

#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace multithreads {

class ThreadPool {
private:
  mutable std::mutex mutex_;
  std::condition_variable condition_variable_;

  std::vector<std::thread> threads_;

  std::queue<std::function<void()>> queue_;

  bool shutdown_requested_ = false;

public:
  ThreadPool(int amount_of_threads) {
    this->threads_.reserve(amount_of_threads);
    for (int i = 0; i < amount_of_threads; ++i) {
      auto worker = [this] {
        while (!this->shutdown_requested_ ||
               (this->shutdown_requested_ && !this->queue_.empty())) {
          std::unique_lock<std::mutex> lock(this->mutex_);
          this->condition_variable_.wait(lock, [this] {
            return this->shutdown_requested_ || !this->queue_.empty();
          });

          if (this->shutdown_requested_ && this->queue_.empty()) {
            return;
          }

          auto task = std::move(this->queue_.front());
          this->queue_.pop();
          lock.unlock();
          task();
        }
      };

      this->threads_.emplace_back(std::move(worker));
    }
  };

  ~ThreadPool() {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->shutdown_requested_ = true;
    lock.unlock();
    this->condition_variable_.notify_all();

    for (auto &worker : this->threads_) {
      worker.join();
    }
  }

  template <class F, typename... Args>
  auto addTask(F &&task, Args &&...args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    auto func = std::bind(std::forward<F>(task), std::forward<Args>(args)...);
    auto packaged_task_ptr = std::make_shared<
        std::packaged_task<std::invoke_result_t<F, Args...>()>>(func);

    auto future = packaged_task_ptr->get_future();

    // wrap the task to get rid of the return value to standardize it
    auto task_wrapper = [packaged_task_ptr] { (*packaged_task_ptr)(); };

    {
      std::lock_guard<std::mutex> lock(this->mutex_);
      this->queue_.emplace(std::move(task_wrapper));
    }
    this->condition_variable_.notify_one();

    return future;
  };
};
} // namespace multithreads
