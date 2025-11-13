@echo off
REM Run script for TrackMate GPS Tracker (Windows)

echo.
echo ╔══════════════════════════════════════════════════════════════╗
echo ║              TrackMate GPS Tracker - Quick Run              ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.

REM Build if executable doesn't exist
if not exist trackmate.exe (
    echo 📦 Executable not found. Building first...
    call build.bat
    if %errorlevel% neq 0 exit /b 1
)

REM Run with defaults: Enhanced network (2), A* algorithm (2)
echo 2 | trackmate.exe 2

echo.
echo 🌐 To visualize the route:
echo    1. Open a web server: python -m http.server 8000
echo    2. Open: http://localhost:8000/index.html
echo.
pause
