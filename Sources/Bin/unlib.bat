@type "%~dp0\MRefal.r5-Not linked.txt" | ^
find /V ":InOut:" | ^
find /V ":MInOut:" | ^
find /V ":MOS:" | ^
find /V ":FileIO:" | ^
find /V ":MFileIO:" | ^
find /V ":MStrings:" | ^
find /V ":MOrder:" | ^
find /V ":MSelfDiag:" | ^
find /V ":MFileSystem:" | ^
find /V ":Math:" | ^
find /V ":MMath:" | ^
find /V ":MDosWinRecoder:" | ^
find /V ":MXXIO:" | ^
find /V ":MSwapSupport:" | ^
find /V ":MLambda:" | ^
find /V ":MProfiler:" > "%~dp0\MRefal.r5-Not linked.txt"
