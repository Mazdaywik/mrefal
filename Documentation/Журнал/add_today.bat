@echo off
echo.>>%1
echo.>>%1
echo.>>%1
echo [%date% - %time%]>>%1
set /P VERSION=<..\Version.txt
echo (Версия %VERSION%)>>%1
echo.>>%1