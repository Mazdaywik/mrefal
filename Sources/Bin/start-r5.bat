@echo off
echo [Profile: Refal 5 (without -t)]
refgo -c300 -l200 ..\Bin\mrefal.r5.rsl ++extent:..\Bin\FS-Extent.cpp.exe %*
if exist ..\Bin\*.tds del ..\Bin\*.tds
if exist *.obj del *.obj