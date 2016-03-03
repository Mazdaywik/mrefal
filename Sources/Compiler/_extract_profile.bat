@echo off

set LOG=%1
if {%LOG%}=={} set LOG=__log.txt

if exist log.tmp erase log.tmp

for /L %%i in (1,1,11) do (
  echo %%i
  mingw32-make profile=R5 mode=BUILD
  call :SCAN_LOG >> log.tmp
  copy _MRefal_profiler.log _MRefal_profiler.log-%LOG%
)

sort log.tmp > %LOG%
erase log.tmp

goto :EOF

:SCAN_LOG
  set LINEPROC=HEADER
  for /F "delims=| tokens=1,2" %%L in (_MRefal_profiler.log) do call :INDIRECT "%%L" "%%M"
goto :EOF

:INDIRECT
  call :%LINEPROC% "%~1" "%~2"
goto :EOF

:HEADER
  echo %~1 | find "Name: FE-MRefal.MLexer.Profiling-NextToken" >NUL
  if not errorlevel 1 (
    set LINEPROC=LINE
  )
goto :EOF

:LINE
  echo %~1^^^|%~2 | find "* Refal time"
  if not errorlevel 1 (
    set LINEPROC=SKIP
  )
goto :EOF

:SKIP
  ::
goto :EOF