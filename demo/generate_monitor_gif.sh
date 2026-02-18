#!/bin/bash
# Generate ThreadPool Monitor GIF from focused demo
# This script runs the demo, creates the GIF, and cleans up temporary files

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
ASSETS_DIR="$SCRIPT_DIR/../assets"
DEMO_EXEC="$BUILD_DIR/demo_focused"

echo "=== ThreadPool Monitor GIF Generator ==="
echo

# Check if demo is built
if [ ! -f "$DEMO_EXEC" ]; then
    echo "Error: Demo executable not found at $DEMO_EXEC"
    echo "Please build the demo first:"
    echo "  cd $SCRIPT_DIR && mkdir -p build && cd build"
    echo "  cmake .. && cmake --build ."
    exit 1
fi

# Clean up any existing temp files
rm -f /tmp/threadpool_frame_*.png

# Start Xvfb (virtual framebuffer)
echo "Starting virtual display..."
Xvfb :99 -screen 0 420x280x24 >/dev/null 2>&1 &
XVFB_PID=$!
export DISPLAY=:99
sleep 2

# Run the demo application
echo "Running demo (15 seconds)..."
timeout 20s "$DEMO_EXEC" >/dev/null 2>&1 || true

# Kill Xvfb
kill $XVFB_PID 2>/dev/null || true

# Check if screenshots were generated
SCREENSHOTS=(/tmp/threadpool_frame_*.png)
if [ ${#SCREENSHOTS[@]} -eq 0 ]; then
    echo "Error: No screenshots generated!"
    exit 1
fi

echo "Captured ${#SCREENSHOTS[@]} frames"

# Create assets directory if it doesn't exist
mkdir -p "$ASSETS_DIR"

# Generate animated GIF
echo "Creating animated GIF..."
convert -delay 50 -loop 0 /tmp/threadpool_frame_*.png "$ASSETS_DIR/threadpool_monitor.gif"

# Clean up temporary files
echo "Cleaning up temporary files..."
rm -f /tmp/threadpool_frame_*.png

# Get file size
GIF_SIZE=$(du -h "$ASSETS_DIR/threadpool_monitor.gif" | cut -f1)

echo
echo "✅ GIF generated successfully!"
echo "  Location: $ASSETS_DIR/threadpool_monitor.gif"
echo "  Size: $GIF_SIZE"
echo "  Frames: ${#SCREENSHOTS[@]}"
echo
echo "The GIF is ready to be used in README.md"
