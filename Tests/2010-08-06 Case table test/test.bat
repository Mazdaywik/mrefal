@echo off
..\Bin\MRefal.cppsr.exe "+cfg:.Config/default-CPPSR.cfg" CaseTableTest
del *.tds
CaseTableTest.exe >__out 2>__err