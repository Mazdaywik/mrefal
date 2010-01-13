@echo off
set PROGRAMS=Grab_info Text-To-HTML TODO_list Recoder VersionUpdater
set PROGRAMS=%PROGRAMS% MergeFindResults ErrorDecoder DocConverter
set PROGRAMS=%PROGRAMS% DocConverter-wp ClearXLinx

refgo rsl\VersionUpdater /verfile:Utilities_version.txt /srcfile:mUtilitiesBanner.mref >nul

if exist _compilation.log del _compilation.log

call compile_mr %PROGRAMS% >> _compilation.log

del *.ref *.tds
rd /s /q ~Defs Info ~ROut ~XLinx .Info ~Cpp-01 ~SimRef

copy *.rsl ..\..\Bin
copy *.exe ..\..\Bin

mkdir exe rsl
move *.exe exe\
move *.rsl rsl\