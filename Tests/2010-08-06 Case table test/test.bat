@echo off
..\..\Sources\Bin\MRefal.cppsr.exe "+cfg:../../Sources/Compiler/.Config/default-CPPSR.cfg CaseTableTest
del *.tds
CaseTableTest.exe >__out 2>__err