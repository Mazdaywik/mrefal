@echo off
set CC=-c "compile_bcc -I\"../Runtime/all.cpp\""
set LIBPATH=-d ../Libraries.sr -d ../Compiler.sr -d ../Runtime/all.cpp
set LIBRARIES=refalrts Library LibraryEx Escape FindFile sUtilitiesBanner.sref
"..\Compiler.sr\srefc" %CC% %LIBPATH% %* %LIBRARIES%
del *.obj *.tds