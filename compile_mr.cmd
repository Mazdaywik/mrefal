@echo off

setlocal
  set mrefal=C:\Documents and Settings\Shurik\Мои документы\Programming\MRefal
  set MP=.;%mrefal%/Libraries/all.all
  set MP=%MP%;%mrefal%/Libraries/all.r5
  set MP=%MP%;%mrefal%/Libraries/windows.all
  set MP=%MP%;%mrefal%/Libraries/windows.r5
  set PATH=%PATH%;%mrefal%\Bin
  refgo -c256 "%mrefal%\Bin\mrefal.rsl" +make+ "=MP:.;%MR_Home%" %*
endlocal