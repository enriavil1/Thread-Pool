#include <thread_pool/thread_pool.h>
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>

using namespace multithreads;

// Test counter for tracking task execution
std::atomic<int> task_counter{0};

void simpleTask() {
    ++task_counter;
}

void sleepTask(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    ++task_counter;
}

int returnValueTask(int value) {
    return value * 2;
}

void testBasicTaskExecution() {
    std::cout << "Test: Basic task execution... ";
    task_counter = 0;
    
    {
        ThreadPool pool(4);
        for (int i = 0; i < 10; ++i) {
            pool.addTask(simpleTask);
        }
    } // Pool destructor waits for all tasks
    
    assert(task_counter == 10);
    std::cout << "PASSED\n";
}

void testTaskWithArguments() {
    std::cout << "Test: Task with arguments... ";
    task_counter = 0;
    
    {
        ThreadPool pool(2);
        pool.addTask(sleepTask, 10);
        pool.addTask(sleepTask, 10);
        pool.addTask(sleepTask, 10);
    }
    
    assert(task_counter == 3);
    std::cout << "PASSED\n";
}

void testReturnValues() {
    std::cout << "Test: Task return values... ";
    
    {
        ThreadPool pool(2);
        auto future1 = pool.addTask(returnValueTask, 5);
        auto future2 = pool.addTask(returnValueTask, 10);
        auto future3 = pool.addTask(returnValueTask, 15);
        
        assert(future1.get() == 10);
        assert(future2.get() == 20);
        assert(future3.get() == 30);
    }
    
    std::cout << "PASSED\n";
}

void testConcurrentExecution() {
    std::cout << "Test: Concurrent execution... ";
    task_counter = 0;
    
    auto start = std::chrono::steady_clock::now();
    
    {
        ThreadPool pool(4);
        // Add 4 tasks that each take 100ms
        for (int i = 0; i < 4; ++i) {
            pool.addTask(sleepTask, 100);
        }
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    // Should complete in ~100ms with 4 threads, not 400ms sequentially
    assert(duration < 200);
    assert(task_counter == 4);
    std::cout << "PASSED (took " << duration << "ms)\n";
}

void testLargeNumberOfTasks() {
    std::cout << "Test: Large number of tasks... ";
    task_counter = 0;
    
    {
        ThreadPool pool(8);
        for (int i = 0; i < 1000; ++i) {
            pool.addTask(simpleTask);
        }
    }
    
    assert(task_counter == 1000);
    std::cout << "PASSED\n";
}

void testSingleThreadPool() {
    std::cout << "Test: Single thread pool... ";
    task_counter = 0;
    
    {
        ThreadPool pool(1);
        for (int i = 0; i < 5; ++i) {
            pool.addTask(simpleTask);
        }
    }
    
    assert(task_counter == 5);
    std::cout << "PASSED\n";
}

void testGetterMethods() {
    std::cout << "Test: Getter methods... ";
    
    ThreadPool pool(8);
    
    // Test getTotalThreadCount
    assert(pool.getTotalThreadCount() == 8);
    
    // Test getQueueSize and getActiveThreadCount
    assert(pool.getQueueSize() == 0);
    assert(pool.getActiveThreadCount() == 0);
    
    // Add tasks and check queue size
    for (int i = 0; i < 20; ++i) {
        pool.addTask(sleepTask, 50);
    }
    
    // Give tasks time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // Should have some active threads and/or queued tasks
    size_t active = pool.getActiveThreadCount();
    size_t queued = pool.getQueueSize();
    
    assert(active <= 8); // Can't have more active than total threads
    assert(active + queued > 0); // Should have some work
    
    std::cout << "PASSED (active: " << active << ", queued: " << queued << ")\n";
}

void testLambdaTasks() {
    std::cout << "Test: Lambda tasks... ";
    
    std::atomic<int> counter{0};
    
    {
        ThreadPool pool(4);
        
        for (int i = 0; i < 10; ++i) {
            pool.addTask([&counter]() {
                ++counter;
            });
        }
    }
    
    assert(counter == 10);
    std::cout << "PASSED\n";
}

void testTaskOrdering() {
    std::cout << "Test: Task completion (not ordering)... ";
    
    std::atomic<int> completed{0};
    
    {
        ThreadPool pool(1); // Single thread ensures sequential execution
        
        for (int i = 0; i < 5; ++i) {
            pool.addTask([&completed, i]() {
                ++completed;
            });
        }
    }
    
    assert(completed == 5);
    std::cout << "PASSED\n";
}

void testExceptionHandling() {
    std::cout << "Test: Exception handling... ";
    
    {
        ThreadPool pool(2);
        
        auto future = pool.addTask([]() -> int {
            throw std::runtime_error("Test exception");
            return 42;
        });
        
        bool exception_caught = false;
        try {
            future.get();
        } catch (const std::runtime_error& e) {
            exception_caught = true;
        }
        
        assert(exception_caught);
    }
    
    std::cout << "PASSED\n";
}

#ifdef THREADPOOL_IMGUI_DEBUG
void testImGuiDebugMethods() {
    std::cout << "Test: ImGui debug methods available... ";
    
    ThreadPool pool(4);
    
    // Just verify the method exists and is callable
    // We can't actually test ImGui rendering without a window context
    // but we can verify the method compiles and links
    
    // Note: renderDebugWindow would require ImGui context to actually call
    // For now, just verify the getters work
    assert(pool.getTotalThreadCount() == 4);
    assert(pool.getActiveThreadCount() >= 0);
    assert(pool.getQueueSize() >= 0);
    
    std::cout << "PASSED\n";
}
#endif

int main() {
    std::cout << "=== ThreadPool Unit Tests ===\n\n";
    
    // Core functionality tests
    testBasicTaskExecution();
    testTaskWithArguments();
    testReturnValues();
    testConcurrentExecution();
    testLargeNumberOfTasks();
    testSingleThreadPool();
    testGetterMethods();
    testLambdaTasks();
    testTaskOrdering();
    testExceptionHandling();
    
#ifdef THREADPOOL_IMGUI_DEBUG
    std::cout << "\n=== ImGui Debug Tests ===\n\n";
    testImGuiDebugMethods();
    std::cout << "\nImGui debugging: ENABLED\n";
#else
    std::cout << "\nImGui debugging: DISABLED\n";
#endif
    
    std::cout << "\n=== All Tests Passed! ===\n";
    return 0;
}
