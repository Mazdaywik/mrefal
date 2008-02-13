@echo off
set PROGRAMS=grab_info text-to-html TODO_list error_parser Recoder VersionUpdater

refgo VersionUpdater /verfile:Utilities_version.txt /srcfile:mUtilitiesBanner.mref >nul

if exist compilation.log del compilation.log

for %%p in ( %PROGRAMS% ) do (
	echo Compiling %%p ...
	call compile_mr %%p.mref >> compilation.log
	echo.
)

del *.ref
rd /s /q Defs Info ROut3