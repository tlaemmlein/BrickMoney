rem @echo off

call %~dp0\scripts\env_cmake.bat

set MyQtVersion=5.15.0
set MyToolchain=win64
echo INFO: Start to configure qt-%MyQtVersion%-%MyToolchain%
call %~dp0\scripts\env_qt.bat %MyToolchain% %MyQtVersion%

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"

set BuildFolder="%~dp0\build\qt-%MyQtVersion%-%MyToolchain%"
if exist %BuildFolder% rmdir %BuildFolder% /q /s

cmake -G "Visual Studio 15 2017" -A x64 -S %~dp0 -B "%BuildFolder%"

timeout /T 3
