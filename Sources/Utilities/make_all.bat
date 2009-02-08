@echo off
set PROGRAMS=Grab_info Text-To-HTML TODO_list Recoder VersionUpdater
set PROGRAMS=%PROGRAMS% MergeFindResults ErrorDecoder

refgo VersionUpdater /verfile:Utilities_version.txt /srcfile:mUtilitiesBanner.mref >nul

if exist _compilation.log del _compilation.log

call compile_mr %PROGRAMS% >> _compilation.log

del *.ref
rd /s /q ~Defs Info ~ROut ~XLinx

copy *.rsl ..\..\Bin