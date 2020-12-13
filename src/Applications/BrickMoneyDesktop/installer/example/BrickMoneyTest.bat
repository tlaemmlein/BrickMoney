
pushd %~dp0

set WixBinFolder=..\..\..\..\..\Tools\Win\WiX\3.11\

%WixBinFolder%candle.exe BrickMoneyTest.wxs
%WixBinFolder%light.exe BrickMoneyTest.wixobj


popd

@pause