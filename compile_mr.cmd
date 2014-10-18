@echo off

setlocal
  set mrefal=C:\Users\Mazdaywik\Documents\Projects\MRefal
  set PATH=%PATH%;%mrefal%\Bin
  "%mrefal%\Bin\mrefal.exe" +cfg:C:\Users\Mazdaywik\Documents\Projects\MRefal\compile_mr.cfg %*
endlocal