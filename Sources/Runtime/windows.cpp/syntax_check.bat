@cls
@set files=baselib.cpp
@echo.
@echo.
g++ -Wall -c %files%
@echo off
call compile_bcc -c -wall %files%
rem Предупреждение № 4800 --- приведение BOOL к bool.
rem Предупреждение № 4061 --- не все значения enum'а упомянуты в switch'е.
rem Утилита find используется для подавления предупреждений внутри библиотеки.
call compile_cl /c /Wall /wd4800 /wd4061 %files% | find /V /I "Microsoft Visual Studio 8"
del *.obj *.o