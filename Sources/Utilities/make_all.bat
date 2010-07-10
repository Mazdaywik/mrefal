@echo off
set PROGRAMS=Grab_info Text-To-HTML TODO_list Recoder VersionUpdater
set PROGRAMS=%PROGRAMS% MergeFindResults ErrorDecoder DocConverter
set PROGRAMS=%PROGRAMS% DocConverter-wp ClearXLinx

..\..\Bin\VersionUpdater.cpp.exe /verfile:Utilities_version.txt /srcfile:mUtilitiesBanner.mref >nul

if exist _compilation.log del _compilation.log

echo y | call compile_mr %PROGRAMS% 2>__err >> _compilation.log

echo.>>_compilation.log
echo STDERR:>>_compilation.log
echo.>>_compilation.log

del __err

del *.ref *.tds
rd /s /q ~Defs Info ~ROut ~XLinx .Info ~Cpp-01 ~SimRef

copy *.rsl ..\..\Bin
copy *.exe ..\..\Bin

mkdir exe rsl
move *.exe exe\
move *.rsl rsl\