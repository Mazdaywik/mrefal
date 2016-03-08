@echo off

setlocal
  set mrefal=%~dp0
  set PATH=%mrefal%\Bin;%PATH%
  mrefal.cpp.exe +MP:. +cfg:%mrefal%\compile_mr.cfg %*
endlocal