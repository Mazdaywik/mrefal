@echo off
set PROGRAMS=Grab_info Text-To-HTML TODO_list Recoder VersionUpdater
set PROGRAMS=%PROGRAMS% MergeFindResults ErrorDecoder

refgo VersionUpdater /verfile:Utilities_version.txt /srcfile:mUtilitiesBanner.mref >nul

if exist compilation.log del compilation.log

for %%p in ( %PROGRAMS% ) do (
  echo Compiling %%p ...
  echo.>> compilation.log
  call compile_mr %%p.mref >> compilation.log
  echo.
)

del *.ref
rd /s /q Defs Info ROut