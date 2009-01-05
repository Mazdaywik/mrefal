@echo off
setlocal
  set libraries=FileIO InOut Math MOrder MOS MSelfDiag MStrings Types MFileSystem MLambda
  set libraries=%libraries% MDosWinRecoder MSwapSupport MXXIO MR-Extent
  for %%s in ( %libraries% ) do copy ..\~Defs\%%s.rsym ~Defs\%%s.rsym
  for %%o in ( %libraries% ) do copy ..\~ROut\%%o.rout ~ROut\%%o.rout
  for %%o in ( %libraries% ) do copy ..\~ROutS\%%o.srout ~ROutS\%%o.srout
endlocal
