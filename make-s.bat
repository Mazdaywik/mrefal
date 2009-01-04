@echo off
echo STABLE
refgo -c128 -t mrefal.s +build+ "=MP:.;MR-Home" MRefal.mref
if not errorlevel 1 (
  refgo Utilities/VersionUpdater /verfile:Info/Version.txt /srcfile:mVersion.mref>nul
) else (
  rem echo Errors. Version not updated.
)