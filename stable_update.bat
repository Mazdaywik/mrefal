@echo off

rmdir /s /q Libraries
mkdir Libraries

rmdir /s /q Runtime
mkdir Runtime

for %%i in (~Defs ~ROut ~XLinx ~SimRef-01 ~Cpp-01 ~Cpp-01-RASL) do (
  for %%p in (all windows) do (
    xcopy /S /I /Y Sources\Libraries\%%p.all\%%i       Libraries\%%p.all\%%i
    xcopy /S /I /Y Sources\Libraries\%%p.cppsr01-b\%%i Libraries\%%p.cppsr01\%%i
    xcopy /S /I /Y Sources\Libraries\%%p.cppsr01-s\%%i Libraries\%%p.cppsr01\%%i
    xcopy /S /I /Y Sources\Libraries\%%p.r5-b\%%i      Libraries\%%p.r5\%%i
  )
)

xcopy /S /I /Y Sources\Runtime\windows.cpp      Runtime\windows.cpp
xcopy /S /I /Y Sources\Runtime\all.cpp          Runtime\all.cpp
copy /Y Sources\Runtime\windows.sr\*.cpp        Runtime\windows.cpp\*.cpp
copy /Y Sources\Runtime\all.sr\*.cpp            Runtime\all.cpp\*.cpp

copy /Y Sources\Bin\mrefal.cppsr.exe            Bin\mrefal.cpp.exe
copy /Y Sources\Bin\mrefal.sr.exe               Bin\mrefal.exe
copy /Y Sources\Bin\MRefal.r5.rsl               Bin\mrefal.rsl
copy /Y Sources\Utilities\exe\FS-Extent.exe     Bin\FS-Extent.cpp.exe
