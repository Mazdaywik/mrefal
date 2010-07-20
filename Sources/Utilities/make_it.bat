@echo off
if "%1"=="" goto :EOF
if "%~x1"==".sref" call compile_sr.bat %*
if "%~x1"==".mref" call compile_mr.bat %*
