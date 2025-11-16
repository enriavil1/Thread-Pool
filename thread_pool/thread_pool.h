#pragma once

#include <thread>
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
        while (!this->shutdown_requested_) {
          std::unique_lock<std::mutex> lock(this->mutex_);
          this->condition_variable_.wait(lock, this->shutdown_requested_ ||
                                                   !this->queue_.empty());
          if (this->shutdown_requested_ || this->queue_.empty())
            return;

          auto task = std::move(this->queue_.front());
          this->queue_.pop();
          lock.unlock();
          task();
        }
      };

      this->threads_.emplace_back(worker);
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

  template <class F> void addTask(F &&task) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->queue_.emplace(std::forward<F>(task));
    lock.unlock();
    this->condition_variable_.notify_one();
  };
};
} // namespace multithreads
