@echo off
"..\Compiler.sr\srefc" -c "call compile_bcc -I\"../Libraries.sr\"" -d ..\Libraries.sr %* refalrts Library WinLib