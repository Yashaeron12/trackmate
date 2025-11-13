@echo off
REM Clean script for TrackMate GPS Tracker (Windows)

echo 🧹 Cleaning build artifacts...

if exist trackmate.exe del trackmate.exe
if exist *.o del *.o
if exist *.obj del *.obj

echo ✅ Clean complete!
