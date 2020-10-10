@echo off

call %~dp0\..\..\scripts\env_cmake.bat

set MyQtVersion=5.15.0
set MyToolchain=win64
echo INFO: Start to configure qt-%MyQtVersion%-%MyToolchain%
call %~dp0\..\..\scripts\env_qt.bat %MyToolchain% %MyQtVersion%
rem -DKDDockWidgets_STATIC=true
cmake -G "Visual Studio 15 2017" -A x64 -S %~dp0/src -B "build/qt-%MyQtVersion%-%MyToolchain%" 
echo INFO: Configuration qt-%MyQtVersion%-%MyToolchain% finished

timeout /T 3
