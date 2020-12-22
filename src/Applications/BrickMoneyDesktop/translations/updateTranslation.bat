@echo off

pushd %~dp0

set MyQtVersion=5.15.0
set MyToolchain=win64
echo INFO: Start to configure qt-%MyQtVersion%-%MyToolchain%
call %~dp0\..\..\..\scripts\env_qt.bat %MyToolchain% %MyQtVersion%

lupdate -no-obsolete %~dp0\..\ %~dp0\..\..\..\Packages -ts bm_de.ts
lrelease bm_de.ts -qm bm_de.qm

echo INFO: SUCCESS Translation is updated

timeout /T 10
