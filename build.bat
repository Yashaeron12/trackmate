@echo off
REM Build script for TrackMate GPS Tracker (Windows)

echo.
echo ╔══════════════════════════════════════════════════════════════╗
echo ║        TrackMate GPS Tracker - Windows Build Script         ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.

REM Check if gcc is available
gcc --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Error: GCC not found! Please install MinGW.
    pause
    exit /b 1
)

echo 📦 Compiling TrackMate modules...
echo.

REM Compile all modules
gcc distance.c heap.c graph.c pathfinding.c json_output.c data_loader.c main.c -o trackmate.exe -std=c99 -Wall -O2 -lm

if %errorlevel% equ 0 (
    echo.
    echo ✅ Build successful!
    echo.
    echo 🚀 Run with: trackmate.exe
    echo 📄 Documentation: MODULAR_STRUCTURE.md
    echo.
) else (
    echo.
    echo ❌ Build failed! Check errors above.
    echo.
    pause
    exit /b 1
)
