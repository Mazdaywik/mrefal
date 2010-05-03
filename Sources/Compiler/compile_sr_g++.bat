@echo off
set srefc="..\..\Simple Refal\Compiler\srefc"
set rtdir=..\Runtime\windows.cpp
set cc="g++ -I%rtdir% -oMRefal.exe"
@%srefc% -c %cc% -d %rtdir% %* refalrts baselib MR-BaseLib
