@echo off
echo [Profile: Refal 5]
refgo -c300 -l200 -t "%~dp0mrefal.r5.rsl" ++extent:"..\Bin\FS-Extent.cpp.exe" %*
if exist "%~dp0*.tds" del "%~dp0*.tds"
if exist *.obj del *.obj
