@echo off
dir /b *.mref>mrefs
refgo "%~dp0grab_info.rsl" @mrefs
del mrefs