@echo off
set PROGRAMS=grab_info text-to-html TODO_list error_parser Recoder

for %%p in ( %PROGRAMS% ) do (
	call compile_mr %%p.mref
	echo.
)

del *.ref
rd /s /q Defs Info ROut3