@echo off
set PATH=%PATH%;..\MR-Extent;..\..\Bin
set MP=.;../Libraries/all.all
set MP=%MP%;../Libraries/all.r5-b
set MP=%MP%;../Libraries/all.r5-s
set MP=%MP%;../Libraries/windows.all
set MP=%MP%;../Libraries/windows.r5-b
set MP=%MP%;../Libraries/windows.r5-s
refgo -c128 -t mrefal +build+ "=MP:%MP%" MRefal.mref
if not errorlevel 1 (
  refgo ../../Bin/VersionUpdater /verfile:../../Version.txt /srcfile:mVersion.mref>nul
  cd ..\..
  call stable_update
  cd Sources\Compiler
) else (
  rem echo Errors. Version not updated.
)