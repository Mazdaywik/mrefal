@for /F "delims=|" %%m in (.gitignore) do @if exist "%%m" erase /S /Q "%%m"
rd .Info