@echo off

echo Setting up a Qt environment...

set MyToolchain=%1

if "%MyToolchain%" == "" (
    set MyToolchain=win64
)

set MyQtVersion=%2

if "%MyQtVersion%" == "" (
    set MyQtVersion=5.15.0
)

set QTDIR=C:\Qt\Qt%MyQtVersion%-%MyToolchain%

echo -- QTDIR set to %QTDIR%
set PATH=%QTDIR%\bin;%PATH%
echo -- Added %QTDIR%\bin to PATH



