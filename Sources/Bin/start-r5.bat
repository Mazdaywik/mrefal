@echo off
echo [Profile: Refal 5 (without -t)]
refgo -c300 -l200 "%~dp0mrefal.r5.rsl" ++extent:"%~dp0FS-Extent.cpp.exe" %*
if exist "%~dp0*.tds" "del %~dp0*.tds"
if exist *.obj del *.obj
