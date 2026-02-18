# CI/CD Setup Instructions

This directory contains the GitHub Actions workflow configuration for the Thread-Pool project.

## Quick Setup

To enable CI/CD for this repository:

1. Create the `.github/workflows` directory in the repository root:
   ```bash
   mkdir -p .github/workflows
   ```

2. Copy the workflow file:
   ```bash
   cp docs/CI-CD/ci.yml .github/workflows/
   ```

3. Commit and push:
   ```bash
   git add .github/workflows/ci.yml
   git commit -m "Add CI/CD workflow"
   git push
   ```

## What the CI/CD Pipeline Does

### 1. Build and Test Matrix
- **Platforms**: Ubuntu, macOS, Windows
- **Build Types**: Release, Debug
- Tests run automatically on every push and pull request
- Ensures cross-platform compatibility

### 2. ImGui Debug Build
- Verifies the optional ImGui debugging feature builds correctly
- Tests on Ubuntu with GLFW and OpenGL dependencies

### 3. Code Coverage
- Generates coverage reports using lcov
- Uploads to Codecov for tracking test coverage

### 4. Static Analysis
- Runs cppcheck on all source files
- Identifies potential issues and code quality problems

## Workflow Triggers

The CI pipeline runs on:
- Every push to the `main` branch
- Every pull request targeting `main`

## Viewing Results

After setup, you can view CI results:
1. Go to the **Actions** tab in your GitHub repository
2. Click on any workflow run to see details
3. Expand jobs to view logs and test results

## Status Badge

Add this badge to your README to show CI status:

```markdown
[![CI](https://github.com/enriavil1/Thread-Pool/actions/workflows/ci.yml/badge.svg)](https://github.com/enriavil1/Thread-Pool/actions/workflows/ci.yml)
```

## Local Testing

Run the same checks locally before pushing:

```bash
# Build and test
cmake -B build -DCMAKE_BUILD_TYPE=Release -DTHREADPOOL_BUILD_TESTS=ON
cmake --build build
cd build && ctest --output-on-failure

# Static analysis
cppcheck --enable=all --std=c++20 thread_pool/ tests/ example/

# Coverage (Linux/macOS)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DTHREADPOOL_BUILD_TESTS=ON -DCMAKE_CXX_FLAGS="--coverage"
cmake --build build
cd build && ctest
lcov --capture --directory . --output-file coverage.info
```

## Requirements

The workflow uses:
- CMake 3.20+
- C++20 compiler
- CTest for testing
- lcov for coverage (Linux)
- cppcheck for static analysis

All dependencies are automatically installed by the workflow on CI runners.

## Customization

You can customize the workflow by editing `ci.yml`:
- Add/remove platforms in the matrix
- Change build types
- Add more jobs (e.g., documentation generation)
- Adjust coverage thresholds
- Configure different static analysis tools

## Troubleshooting

**Workflow not running?**
- Ensure the file is in `.github/workflows/` directory
- Check that the file has `.yml` or `.yaml` extension
- Verify the workflow syntax is valid

**Tests failing?**
- Check the Actions tab for detailed logs
- Run tests locally to reproduce
- Review test output in the workflow logs

**Permission issues?**
- Ensure GitHub Actions is enabled in repository settings
- Check that the repository has workflow permissions enabled
