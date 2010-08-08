@echo off
echo [Profile: Simple Refal]
if exist MRefal.exe move MRefal.exe ..\Bin\MRefal.sr.exe
copy ..\Bin\MRefal.sr.exe ..\Bin\MRefal_.sr.exe
..\Bin\MRefal_.sr.exe %*
if exist ..\Bin\*.tds del ..\Bin\*.tds
if exist *.obj del *.obj