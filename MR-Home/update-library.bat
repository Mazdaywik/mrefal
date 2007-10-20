@echo off
setlocal
	set libraries=FileIO InOut Math MOrder MOS MSelfDiag MStrings Types MFileSystem
	for %%s in ( %libraries% ) do copy ..\Defs\%%s.rsym Defs\%%s.rsym
	rem for %%o in ( %libraries% ) do copy ..\ROut\%%o.rout2 ROut\%%o.rout2
	for %%o in ( %libraries% ) do copy ..\ROut2\%%o.rout2 ROut2\%%o.rout2
endlocal
