@echo off
echo [Profile: Simple Refal]
copy ..\Bin\MRefal.sr.exe ..\Bin\MRefal_.sr.exe
..\Bin\MRefal_.sr.exe %*
if exist ..\Bin\*.tds del ..\Bin\*.tds
if exist *.obj del *.obj