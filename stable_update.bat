@echo off
for %%i in (~Defs ~ROut ~XLinx ~SimRef) do (
  xcopy /S /I /Y Sources\Libraries\all.all\%%i      Libraries\all.all\%%i
  xcopy /S /I /Y Sources\Libraries\all.r5-b\%%i     Libraries\all.r5\%%i
  xcopy /S /I /Y Sources\Libraries\all.r5-s\%%i     Libraries\all.r5\%%i
  xcopy /S /I /Y Sources\Libraries\all.sr-b\%%i     Libraries\all.sr\%%i
  xcopy /S /I /Y Sources\Libraries\all.sr-s\%%i     Libraries\all.sr\%%i
  xcopy /S /I /Y Sources\Libraries\windows.all\%%i  Libraries\windows.all\%%i
  xcopy /S /I /Y Sources\Libraries\windows.r5-b\%%i Libraries\windows.r5\%%i
  xcopy /S /I /Y Sources\Libraries\windows.r5-s\%%i Libraries\windows.r5\%%i
  xcopy /S /I /Y Sources\Libraries\windows.sr-b\%%i Libraries\windows.sr\%%i
  xcopy /S /I /Y Sources\Libraries\windows.sr-s\%%i Libraries\windows.sr\%%i
)
copy /Y Sources\Compiler\mrefal.exe Bin\mrefal.exe
copy /Y Sources\Compiler\mrefal.rsl Bin\mrefal.rsl
copy /Y Sources\MR-Extent\mr-extent.exe Bin\mr-extent.exe