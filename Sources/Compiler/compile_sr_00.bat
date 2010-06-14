@echo off
set srefc="..\..\Simple Refal\Compiler\srefc"
set rtdir=..\Runtime\windows.cpp
@%srefc% -d %rtdir% %* refalrts baselib MR-BaseLib
