@echo off

pushd %~dp0

set WixBinFolder=..\..\..\..\..\Tools\Win\WiX\3.11\

%WixBinFolder%\candle.exe %~dp0\BrickMoneySetup.wxs -out %~dp0\BrickMoneySetup.wixobj -v
if "%errorlevel%" NEQ "0" (
	echo ERROR: -
	echo ERROR: ********** ERROR: Error while processing candle.exe ***********
	echo ERROR: -
	goto:eof
)
%WixBinFolder%\light.exe -ext WixUIExtension %~dp0\BrickMoneySetup.wixobj -out %~dp0\BrickMoneySetup.msi -v
if "%errorlevel%" NEQ "0" (
	echo ERROR: -
	echo ERROR: ********** ERROR: Error while processing light.exe ***********
	echo ERROR: -
	goto:eof
)

echo INFO: SUCCESS BrickMoneySetup.msi is ready for installing

timeout /T 3
