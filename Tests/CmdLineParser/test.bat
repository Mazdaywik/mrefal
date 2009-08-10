@echo off
del test.cpp.exe
..\..\Sources\Compiler\MRefal.cpp.exe +cfg:test.prj
echo ==========================================================
call run.bat
