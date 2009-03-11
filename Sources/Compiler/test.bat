rem @echo off
set MP=+MP:. +MP:../Libraries/all.all
set MP=%MP% +MP:../Libraries/all.r5-b
set MP=%MP% +MP:../Libraries/all.r5-s
set MP=%MP% +MP:../Libraries/windows.all
set MP=%MP% +MP:../Libraries/windows.r5-b
set MP=%MP% +MP:../Libraries/windows.r5-s
refgo -c150 mrefal.rsl +make +xlinx +be:R5 %MP% MXLinx.mref