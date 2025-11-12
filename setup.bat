@echo off
REM TrackMate GPS Tracker - Windows Setup Script

echo 🗺️  TrackMate GPS Tracker - Windows Setup
echo ==========================================

REM Check if GCC is available
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ GCC not found. Please install MinGW or Visual Studio
    echo    Download from: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo ✅ GCC compiler found

REM Compile the C backend
echo 🔧 Compiling TrackMate backend...
gcc -o trackmate.exe trackmate.c -lm

if %errorlevel% neq 0 (
    echo ❌ Compilation failed!
    pause
    exit /b 1
)

echo ✅ Backend compiled successfully!

REM Run the backend to generate route data
echo 🚀 Generating route data...
trackmate.exe

if not exist "route_data.json" (
    echo ❌ Route data generation failed!
    pause
    exit /b 1
)

echo ✅ Route data generated successfully!

REM Check if Python is available
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ Python not found for web server
    echo 💡 You can still open index.html directly in your browser
    pause
    exit /b 1
)

echo ✅ Python found

REM Start web server
echo 🌐 Starting web server on http://localhost:8080
echo 📍 Open your browser and navigate to: http://localhost:8080
echo 🛑 Press Ctrl+C to stop the server
echo.

python -m http.server 8080