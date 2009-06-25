@echo off
if "%1"=="" goto :exit
.Config\change_window cfg.%1
echo %MODE%