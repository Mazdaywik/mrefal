@echo off
echo [Profile: Simple Refal]
copy %~dp0MRefal.sr.exe %~dp0MRefal_.sr.exe
%~dp0MRefal_.sr.exe %*
if exist %~dp0*.tds del %~dp0*.tds
if exist *.obj del *.obj
