@echo off

setlocal
  set mrefal=%~dp0
  set PATH=%PATH%;%mrefal%\Bin
  "%mrefal%\Bin\mrefal.exe" +cfg:%mrefal%\compile_mr.cfg %*
endlocal