@echo off
echo [Profile: Simple Refal]
if exist MRefal.exe move MRefal.exe MRefal.sr.exe
copy MRefal.sr.exe MRefal_.sr.exe
MRefal_.sr.exe %*
if exist *.tds del *.tds
if exist *.obj del *.obj