@echo off

REM Change directory to where this file is located
cd /d "%~dp0"

REM Delete build dir (for a clean setup)
if exist "build" del /s /f /q "build"

REM Create build dir and enter it
mkdir build
cd build

REM Run CMake, should generate a Visual Studio solution by default (uses newest version installed)
"C:\Program Files\CMake\bin\cmake.exe" -A Win32 ..
pause
