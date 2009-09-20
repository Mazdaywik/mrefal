@echo off
set PATH=%PATH%;..\MR-Extent;..\..\Bin
mrefal.cpp.exe +build +cfg:mp-work-CPPSR.cfg +xlinx MRefal.mref
refgo -c128 -t mrefal +build+ "=MP:%MP%" MRefal.mref
if not errorlevel 1 (
  refgo ../../Bin/VersionUpdater /verfile:../../Version.txt /srcfile:mVersion.mref>nul
  cd ..\..
  call stable_update
  cd Sources\Compiler
) else (
  rem echo Errors. Version not updated.
)