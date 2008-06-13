@echo off
setlocal
	set libraries=FileIO InOut Math MOrder MOS MSelfDiag MStrings Types MFileSystem
	set libraries=%libraries% MDosWinRecoder MSwapSupport
	for %%s in ( %libraries% ) do copy ..\Defs\%%s.rsym Defs\%%s.rsym
	for %%o in ( %libraries% ) do copy ..\ROut\%%o.rout ROut\%%o.rout
	for %%o in ( %libraries% ) do copy ..\ROutS\%%o.routS ROutS\%%o.routS
endlocal
