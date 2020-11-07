rem @echo off

call %~dp0\..\..\scripts\env_cmake.bat

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"

set BuildFolder="%~dp0\build\win64-cl-15"
if exist %BuildFolder% rmdir %BuildFolder% /q /s

cmake -G "Visual Studio 15 2017" -A x64 -S %~dp0 -B "%BuildFolder%"

timeout /T 3
