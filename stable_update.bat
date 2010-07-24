@echo off

rmdir /s /q Libraries
mkdir Libraries

rmdir /s /q Runtime
mkdir Runtime

for %%i in (~Defs ~ROut ~XLinx ~SimRef-01 ~Cpp-01) do (
  for %%p in (all windows) do (
    xcopy /S /I /Y Sources\Libraries\%%p.all\%%i       Libraries\%%p.all\%%i
    xcopy /S /I /Y Sources\Libraries\%%p.cppsr01-b\%%i Libraries\%%p.cppsr01\%%i
    xcopy /S /I /Y Sources\Libraries\%%p.cppsr01-s\%%i Libraries\%%p.cppsr01\%%i
  )
)

xcopy /S /I /Y Sources\Runtime\windows.cpp Runtime\windows.cpp

copy /Y Sources\Compiler\mrefal.cppsr.exe Bin\mrefal.cpp.exe
copy /Y Sources\Compiler\mrefal.sr.exe Bin\mrefal.exe
copy /Y Sources\Compiler\mrefal.rsl Bin\mrefal.rsl
copy /Y Sources\Compiler.sr\srefc.exe Bin\srefc.exe
copy /Y Sources\Utilities\exe\FS-Extent.cpp.exe Bin\FS-Extent.cpp.exe
