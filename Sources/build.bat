@echo off
set PATH=%PATH%;..\MR-Extent;..\..\Bin

set VERFILE=..\..\Version.txt
set VERSRC=mVersion.mref
set /P CURVER=<%VERFILE%
set LOGFILE=~rebuild.log

rem ===========================================================================
rem Сборка Модульного Рефала
rem ===========================================================================

pushd Compiler
setlocal
set LOGFILE=..\%LOGFILE%

echo Rebuilding Module Refal (start version %CURVER%)>%LOGFILE%
echo.>>%LOGFILE%

call :SUB_DUAL_PRINT "==========================================" %LOGFILE%
call :SUB_DUAL_PRINT "============ Test for errors. ============" %LOGFILE%
call :SUB_DUAL_PRINT "==========================================" %LOGFILE%
echo.>>%LOGFILE%

call :SUB_CALL_COMPILER cpp  CPPSR %LOGFILE%
if errorlevel 1 goto ERRORS
if exist __errors goto ERRORS
call :SUB_CALL_COMPILER sr   SR    %LOGFILE%
if errorlevel 1 goto ERRORS
if exist __errors goto ERRORS
call :SUB_CALL_COMPILER r5-t R5    %LOGFILE%
if errorlevel 1 goto ERRORS
if exist __errors goto ERRORS

call :SUB_DUAL_PRINT "=========================================" %LOGFILE%
call :SUB_DUAL_PRINT "== Checking successed. Update version. ==" %LOGFILE%
call :SUB_DUAL_PRINT "=========================================" %LOGFILE%
echo.>>%LOGFILE%

copy %VERFILE% %VERFILE%.new
copy %VERSRC% %VERSRC%.bak
refgo ../../Bin/VersionUpdater /verfile:%VERFILE%.new /srcfile:%VERSRC% > nul

call :SUB_DUAL_PRINT "==========================================" %LOGFILE%
call :SUB_DUAL_PRINT "=========== Final recompiling. ===========" %LOGFILE%
call :SUB_DUAL_PRINT "==========================================" %LOGFILE%
echo.>>%LOGFILE%

for %%c in (cpp sr r5-t) do (
  for %%p in (CPPSR SR R5) do (
    call :SUB_CALL_COMPILER %%c %%p %LOGFILE%
    if errorlevel 1 goto UNEXPECTED_ERROR
    if exist __errors goto UNEXPECTED_ERROR
  )
)

call :SUB_DUAL_PRINT "=========================================" %LOGFILE%
call :SUB_DUAL_PRINT "=========== Updating release. ===========" %LOGFILE%
call :SUB_DUAL_PRINT "=========================================" %LOGFILE%
echo.>>%LOGFILE%

cd ..\..
call stable_update
cd Sources\Compiler

del %VERFILE%
move %VERFILE%.new %VERFILE%
del %VERSRC%.bak

call :SUB_DUAL_PRINT "===========================================" %LOGFILE%
call :SUB_DUAL_PRINT "========== Rebuilding successed. ==========" %LOGFILE%
call :SUB_DUAL_PRINT "===========================================" %LOGFILE%
echo.>>%LOGFILE%

endlocal
popd

goto :EOF

:ERRORS

call :SUB_DUAL_PRINT "===========================================" %LOGFILE%
call :SUB_DUAL_PRINT "=== FOUND ERRORS! CAN'T UPDATE VERSION! ===" %LOGFILE%
call :SUB_DUAL_PRINT "===========================================" %LOGFILE%
echo.>>%LOGFILE%

endlocal
popd

goto :EOF

:UNEXPECTED_ERROR

del %VERFILE%.new
del %VERSRC%
move %VERSRC%.bak %VERSRC%

call :SUB_DUAL_PRINT "================================================" %LOGFILE%
call :SUB_DUAL_PRINT "== INTERNAL ERROR WHILE RECOMPILING! EXITING! ==" %LOGFILE%
call :SUB_DUAL_PRINT "================================================" %LOGFILE%
echo.>>%LOGFILE%

endlocal
popd

goto :EOF

rem ===========================================================================
rem Подпрограмма для сохранения вывода команды и записи в лог.
rem ===========================================================================
:SUB_CALL_AND_SAVE_OUTPUT
setlocal
  rem %1 --- команда
  rem %2 --- файл вывода
  echo Performing %~1
  echo Perforimg %~1>> %2
  echo.>>%2
  %~1 1>~output 2>~error
  echo ==STDOUT==>>%2
  type ~output>>%2
  echo.>>%2
  echo ==STDERR==>>%2
  type ~error>>%2
  echo.>>%2
  del ~output
  del ~error
endlocal
goto :EOF


rem ===========================================================================
rem Подпрограмма запуска перекомпиляции заданным компилятором с заданным профилем
rem ===========================================================================
:SUB_CALL_COMPILER
setlocal
  rem %1 --- компилятор
  rem %2 --- профиль
  rem %3 --- файл вывода
  call :SUB_DUAL_PRINT "----------------------------------------------------------" %3
  call :SUB_DUAL_PRINT "-- Recompiling with compiler '%1' with profile '%2' --"     %3
  call :SUB_DUAL_PRINT "----------------------------------------------------------" %3
  set CMDLINE=call start-%1.bat +build +cfg:mp-work-%2.cfg +xlinx MRefal.mref
  call :SUB_CALL_AND_SAVE_OUTPUT "%CMDLINE%" %3
endlocal
goto :EOF

rem ===========================================================================
rem Подпрограмма одновременного вывода в лог и на экран
rem ===========================================================================
:SUB_DUAL_PRINT
setlocal
  echo %~1
  echo %~1>>%2
endlocal
goto :EOF
