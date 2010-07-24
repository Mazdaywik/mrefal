@echo [Profile: Refal 5]
refgo -c300 -l200 -t mrefal %*
if exist *.tds del *.tds
if exist *.obj del *.obj