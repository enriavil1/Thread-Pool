# ThreadPool Tests

Comprehensive unit tests for the ThreadPool library.

## Running Tests

### From Repository Root

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

Or run the test executable directly:

```bash
./tests/thread_pool_tests
```

### From Tests Directory

```bash
cd tests
mkdir build && cd build
cmake ..
cmake --build .
./thread_pool_tests
```

## Test Coverage

The test suite covers:

### Core Functionality
- **Basic task execution**: Verify tasks execute correctly
- **Task with arguments**: Test passing arguments to tasks
- **Return values**: Verify futures work correctly
- **Concurrent execution**: Ensure parallel execution works
- **Large number of tasks**: Stress test with 1000 tasks
- **Single thread pool**: Test edge case with 1 thread

### API Tests
- **Getter methods**: Test `getActiveThreadCount()`, `getTotalThreadCount()`, `getQueueSize()`
- **Lambda tasks**: Verify lambda functions work as tasks
- **Task ordering**: Verify tasks complete (ordering not guaranteed for multi-threaded)
- **Exception handling**: Test exception propagation through futures

### ImGui Debug Tests
- **Debug methods availability**: Verify debug methods exist when `THREADPOOL_IMGUI_DEBUG` is enabled

## Test Results

All tests should pass with output similar to:

```
=== ThreadPool Unit Tests ===

Test: Basic task execution... PASSED
Test: Task with arguments... PASSED
Test: Task return values... PASSED
Test: Concurrent execution... PASSED (took 100ms)
Test: Large number of tasks... PASSED
Test: Single thread pool... PASSED
Test: Getter methods... PASSED (active: 8, queued: 12)
Test: Lambda tasks... PASSED
Test: Task completion (not ordering)... PASSED
Test: Exception handling... PASSED

ImGui debugging: DISABLED

=== All Tests Passed! ===
```

## Building Options

### Disable Tests

```bash
cmake -DTHREADPOOL_BUILD_TESTS=OFF ..
```

### With ImGui Debug Enabled

To test with ImGui debugging enabled, you would need to provide ImGui and link it appropriately. The current tests verify the debug methods exist but don't test actual rendering (which would require an ImGui context).

## CI/CD Integration

Tests can be integrated into CI/CD pipelines:

```bash
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

Exit code will be 0 if all tests pass, non-zero otherwise.
