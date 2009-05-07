@set msvc=C:\Program Files\Microsoft Visual Studio 8
@copy "%msvc%\Common7\IDE\mspdb80.dll" mspdb80.dll
@"%msvc%\VC\bin\"cl.exe /Os /I"%mcvc%\VC\PlatformSDK\Include" /I"%msvc%\VC\include" /Zc:forScope /wd4996  %*  /EHsc /link /LIBPATH:"%msvc%\VC\lib" /LIBPATH:"%mcvc%\VC\PlatformSDK\lib" >nul
@erase mspdb80.dll