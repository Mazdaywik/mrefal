@echo off
set srefc="..\..\Simple Refal\Compiler\srefc"
set rtdir=..\Runtime\windows.cpp
set cc="call compile_bcc -I%rtdir% -eMRefal.exe"
@%srefc% -c %cc% -d %rtdir% %* refalrts baselib MR-BaseLib