@echo off
echo [Profile: Refal 5 (without -t)]
refgo -c300 -l200 mrefal %*
if exist *.tds del *.tds
if exist *.obj del *.obj