@echo off
echo [Profile: C++/SR]
if exist MRefal.cpp.exe move MRefal.cpp.exe ..\Bin\MRefal.cppsr.exe
copy ..\Bin\MRefal.cppsr.exe ..\Bin\MRefal_.cppsr.exe
..\Bin\MRefal_.cppsr.exe %*
if exist ..\Bin\*.tds del ..\Bin\*.tds
if exist *.obj del *.obj