@set msvc=C:\Program Files\Microsoft Visual Studio 8
@copy "%msvc%\Common7\IDE\mspdb80.dll" mspdb80.dll
@"%msvc%\VC\bin\"cl.exe /wd4996 /Os /I"%msvc%\VC\PlatformSDK\Include" /I"%msvc%\VC\include" /Zc:forScope  %*  /EHsc /link /LIBPATH:"%msvc%\VC\lib" /LIBPATH:"%mcvc%\VC\PlatformSDK\lib"
@erase mspdb80.dll