#!/bin/bash

# Start Xvfb (virtual framebuffer)
Xvfb :99 -screen 0 1280x720x24 &
XVFB_PID=$!
export DISPLAY=:99

# Wait for Xvfb to start
sleep 2

# Record the demo application
cd /home/ubuntu/Thread-Pool/demo/build

# Run the app for 30 seconds and capture video
timeout 30s ffmpeg -y -f x11grab -video_size 1280x720 -framerate 30 -i :99.0 \
    -c:v libx264 -preset ultrafast -pix_fmt yuv420p \
    /home/ubuntu/Thread-Pool/demo/threadpool_demo.mp4 &
FFMPEG_PID=$!

# Start the demo app
./demo_app &
APP_PID=$!

# Wait for recording to finish
wait $FFMPEG_PID

# Kill the app if still running
kill $APP_PID 2>/dev/null || true

# Kill Xvfb
kill $XVFB_PID

echo "Recording complete: /home/ubuntu/Thread-Pool/demo/threadpool_demo.mp4"
