
pushd %~dp0

set WixBinFolder=..\..\..\..\..\Tools\Win\WiX\3.11\

%WixBinFolder%candle.exe example.wxs
%WixBinFolder%light.exe example.wixobj


popd

@pause