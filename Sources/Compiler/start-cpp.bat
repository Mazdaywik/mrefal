@echo off
echo [Profile: C++/SR]
if exist MRefal.cpp.exe move MRefal.cpp.exe MRefal.cppsr.exe
copy MRefal.cppsr.exe MRefal_.cppsr.exe
MRefal_.cppsr.exe %*
if exist *.tds del *.tds
if exist *.obj del *.obj