@echo off
set MR-PROGRAMS=Grab_info Text-To-HTML TODO_list Recoder VersionUpdater
set MR-PROGRAMS=%MR-PROGRAMS% ErrorDecoder DocConverter
set MR-PROGRAMS=%MR-PROGRAMS% DocConverter-wp ClearXLinx
set MR-PROGRAMS=%MR-PROGRAMS% SRPrep FS-Extent

set VERSRC=/srcfile:mUtilitiesBanner.mref

refgo ..\..\Bin\VersionUpdater.rsl /verfile:Utilities_version.txt %VERSRC%

if exist _compilation.log del _compilation.log
if exist __err del __err

echo y | call compile_mr %MR-PROGRAMS% 2>>__err >> _compilation.log

echo.>>_compilation.log
echo STDERR:>>_compilation.log
echo.>>_compilation.log
type __err>>_compilation.log

del __err

del *.ref *.tds *.cpp
rd /s /q ~Defs Info ~ROut ~XLinx .Info ~Cpp-01 ~SimRef exe rsl

pushd SRPrep
rd /s /q ~Defs Info ~ROut ~XLinx .Info ~Cpp-01 ~SimRef
popd

copy *.rsl ..\..\Bin
copy *.exe ..\..\Bin

mkdir exe rsl
move *.exe exe\
move *.rsl rsl\