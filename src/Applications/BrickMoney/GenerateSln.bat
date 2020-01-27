@echo off

call %~dp0\..\..\scripts\env_cmake.bat
call %~dp0\..\..\scripts\env_qt.bat

cmake -G "Visual Studio 15 2017" -A x64 -S %~dp0 -B "build-cl15-x64"


timeout /T 3
rem cmake --build build32 --config Release

rem cmake --build build64 --config Release
