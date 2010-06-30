@echo off
cls
del test.cpp.exe
del test.exe
del test.rsl
..\..\Sources\Compiler\MRefal.cpp.exe +cfg:test.prj
echo ==========================================================
call run.bat
