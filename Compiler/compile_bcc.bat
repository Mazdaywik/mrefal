rem @echo off
setlocal
bcc32 -I"C:\Borland\BCC55\Include" -L"C:\Borland\BCC55\Lib" %*
del *.obj
endlocal