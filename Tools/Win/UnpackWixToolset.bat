@echo off

pushd %~dp0\WIX

..\7-Zip\7z.exe x 3.11.zip -o3.11
dir

rem x %~dp0\WIX\3.11.zip

popd


