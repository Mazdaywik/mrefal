@echo off
setlocal
	set libraries=FileIO InOut Math MOrder MOS MSelfDiag MStrings Types MFileSystem
	set libraries=%libraries% MDosWinRecoder
	for %%s in ( %libraries% ) do copy ..\Defs\%%s.rsym Defs\%%s.rsym
	for %%o in ( %libraries% ) do copy ..\ROut\%%o.rout ROut\%%o.rout
	for %%o in ( %libraries% ) do copy ..\ROut2\%%o.rout2 ROut2\%%o.rout2
	for %%o in ( %libraries% ) do copy ..\ROut3\%%o.rout3 ROut3\%%o.rout3
endlocal
