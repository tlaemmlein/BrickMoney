@echo off


set PATH=%~dp0\..\..\..\Tools\Win;%PATH%
where ninja
ninja --version

call %~dp0\..\..\scripts\env_cmake.bat

set MyQtVersion=5.15.0
set MyToolchain=win64
echo INFO: Start to configure qt-%MyQtVersion%-%MyToolchain%
call %~dp0\..\..\scripts\env_qt.bat %MyToolchain% %MyQtVersion%

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"

set InstallFolder=%~dp0\..\..\Thirdparty\KDDockWidgets\1.1.95\cl15-x64
if exist %InstallFolder% rmdir %InstallFolder% /q /s

echo INFO: Start to build and install release
set BuildFolder=%~dp0\build\qt-%MyQtVersion%-%MyToolchain%-ninja
if exist %BuildFolder% rmdir %BuildFolder% /q /s
cmake -DCMAKE_BUILD_TYPE=Release -DKDDockWidgets_EXAMPLES=false -G "Ninja" -S %~dp0 -B "%BuildFolder%" src
cmake --build %BuildFolder%
cmake --install %BuildFolder% --prefix %InstallFolder%%

echo INFO: Start to build and install debug
set BuildFolder=%~dp0\build\qt-%MyQtVersion%-%MyToolchain%-ninja-debug
if exist %BuildFolder% rmdir %BuildFolder% /q /s
cmake -DCMAKE_BUILD_TYPE=Debug -DKDDockWidgets_EXAMPLES=false -G "Ninja" -S %~dp0 -B "%BuildFolder%" src
cmake --build %BuildFolder%
cmake --install %BuildFolder% --prefix %InstallFolder%%


rem exit /B 0

timeout /T 3
