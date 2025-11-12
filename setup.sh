#!/bin/bash

# TrackMate GPS Tracker - Quick Setup Script
# Automatically compiles backend and starts web server

echo "🗺️  TrackMate GPS Tracker - Quick Setup"
echo "========================================"

# Check if GCC is available
if ! command -v gcc &> /dev/null; then
    echo "❌ GCC not found. Please install GCC compiler."
    echo "   On Windows: Install MinGW or Visual Studio"
    echo "   On macOS: Install Xcode Command Line Tools"
    echo "   On Linux: sudo apt install gcc"
    exit 1
fi

echo "✅ GCC compiler found"

# Compile the C backend
echo "🔧 Compiling TrackMate backend..."
gcc -o trackmate trackmate.c -lm

if [ $? -eq 0 ]; then
    echo "✅ Backend compiled successfully!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

# Run the backend to generate route data
echo "🚀 Generating route data..."
./trackmate > /dev/null 2>&1

if [ -f "route_data.json" ]; then
    echo "✅ Route data generated successfully!"
else
    echo "❌ Route data generation failed!"
    exit 1
fi

# Check if Python is available for web server
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    PYTHON_CMD="python"
else
    echo "❌ Python not found for web server"
    echo "💡 You can still open index.html directly in your browser"
    exit 1
fi

echo "✅ Python found: $PYTHON_CMD"

# Start web server
echo "🌐 Starting web server on http://localhost:8080"
echo "📍 Open your browser and navigate to: http://localhost:8080"
echo "🛑 Press Ctrl+C to stop the server"
echo ""

$PYTHON_CMD -m http.server 8080