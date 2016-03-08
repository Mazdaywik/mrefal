@echo off
echo [Profile: C++/SR]
copy %~dp0MRefal.cppsr.exe %~dp0MRefal_.cppsr.exe
%~dp0MRefal_.cppsr.exe %*
if exist %~dp0*.tds del %~dp0*.tds
if exist *.obj del *.obj
