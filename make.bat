@echo off
refgo -c128 mrefal MRefal.mref %1
echo.
if not errorlevel 1 (
	refgo Utilities/VersionUpdater /verfile:Info/Version.txt /srcfile:mVersion.mref
) else (
	echo Errors. Version not updated.
)