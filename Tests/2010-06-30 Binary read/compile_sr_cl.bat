@echo off
set srefc="..\..\Simple Refal\Compiler\srefc"
set rtdir=..\..\Sources\Runtime\windows.cpp
set cc="call compile_cl /I%rtdir% /FeTest.cpp.exe"
%srefc% -c %cc% -d %rtdir% %* refalrts baselib MR-BaseLib
