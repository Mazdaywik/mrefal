@..\lexgen\lexgen Lexer.sref
@copy srefc.exe srefc_.exe
@set modules=Srefc.sref Driver.sref Lexer.sref Algorithm.sref
@set modules=%modules% Error.sref Generator.sref SymTable.sref
@set modules=%modules% FindFile ParseCmdLine
@set modules=%modules% Library LibraryEx refalrts.cpp
@srefc.s -c "compile_cl /I..\SRLib /wd4996" -d ..\SRLib %modules%
@del *.obj
@del *.tds
