@echo off
refgo -c128 mrefal MRefal.mref %1
if not errorlevel 1 (
	refgo Utilities/VersionUpdater /verfile:Info/Version.txt /srcfile:mVersion.mref>nul
) else (
	rem echo Errors. Version not updated.
)