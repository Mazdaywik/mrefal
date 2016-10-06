rd /s /q bin doc srlib
erase c-plus-plus.conf.bat

xcopy /S /I /Y ..\simple-refal-distrib\bin bin
xcopy /S /I /Y ..\simple-refal-distrib\doc doc
xcopy /S /I /Y ..\simple-refal-distrib\srlib srlib
copy ..\simple-refal-distrib\c-plus-plus.conf.bat .