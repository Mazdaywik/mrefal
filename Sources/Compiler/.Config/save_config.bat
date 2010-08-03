@echo off
if "%1"=="" goto :exit
setlocal
  set filename=.Config\cfg.%1.wnd
  echo L:  Загрузить %1>%filename%
  if NOT "%COMP-SHORT%"== "" (
    echo     set COMP-SHORT=%COMP-SHORT%>>%filename%
  )
  if NOT "%COMP%"== "" (
    echo     set COMP=%COMP%>>%filename%
  )
  if NOT "%PROFILE-SHORT%"== "" (
    echo     set PROFILE-SHORT=%PROFILE-SHORT%>>%filename%
  )
  if NOT "%PROFILE%"== "" (
    echo     set PROFILE=%PROFILE%>>%filename%
  )
  if NOT "%PROFILE-ACRO%"== "" (
    echo     set PROFILE-ACRO=%PROFILE-ACRO%>>%filename%
  )
  if NOT "%SCOMP-SHORT%"== "" (
    echo     set SCOMP-SHORT=%SCOMP-SHORT%>>%filename%
  )
  if NOT "%SCOMP%"== "" (
    echo     set SCOMP=%SCOMP%>>%filename%
  )
  if NOT "%SPROFILE-SHORT%"== "" (
    echo     set SPROFILE-SHORT=%SPROFILE-SHORT%>>%filename%
  )
  if NOT "%SPROFILE%"== "" (
    echo     set SPROFILE=%SPROFILE%>>%filename%
  )
  if NOT "%MODE%"== "" (
    echo     set MODE=%MODE%>>%filename%
  )
  if NOT "%XLINX%"== "" (
    echo     set XLINX=%XLINX%>>%filename%
  )
  echo     .Config\change_window default>>%filename%
  echo X:  Возврат без загрузки>>%filename%
  echo     .Config\change_window default>>%filename%
endlocal