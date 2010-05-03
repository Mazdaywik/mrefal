@echo off
set PATH=%PATH%;..\MR-Extent;..\..\Bin

echo ------------------------------------------------
echo -- Test for errors (compiler/profile C++/SR). --
echo ------------------------------------------------

call start-cpp.bat +build +cfg:mp-work-CPPSR.cfg +xlinx MRefal.mref
if errorlevel 1 goto ERRORS

echo ------------------------------------------------------
echo -- Test for errors (compiler/profile Simple Refal). --
echo ------------------------------------------------------

call start-sr.bat +build +cfg:mp-work-SR.cfg +xlinx MRefal.mref
if errorlevel 1 goto ERRORS

echo -------------------------------------------------
echo -- Test for errors (compiler/profile Refal 5). --
echo -------------------------------------------------

call start-r5.bat +build +cfg:mp-work-R5.cfg +xlinx MRefal.mref
if errorlevel 1 goto ERRORS

echo -----------------------------------------
echo -- Checking successed. Update version. --
echo -----------------------------------------

refgo ../../Bin/VersionUpdater /verfile:../../Version.txt /srcfile:mVersion.mref > nul

echo ------------------------
echo -- Final recompiling. --
echo ------------------------

for %%c in (cpp sr r5) do (
  for %%p in (CPPSR SR R5) do (
    echo ----------------------------------------------------------
    echo -- Recompiling with compiler '%%c' with profile '%%p' --
    echo ----------------------------------------------------------

    call start-%%c.bat +build +cfg:mp-work-%%p.cfg +xlinx MRefal.mref

    if errorlevel 1 goto UNEXPECTED_ERROR
  )
)

echo -----------------------
echo -- Updating release. --
echo -----------------------

cd ..\..
call stable_update
cd Sources\Compiler

echo ---------------------------
echo -- Rebuilding successed. --
echo ---------------------------

goto :EOF

:ERRORS

echo -----------------------------------------
echo -- FOUND ERRORS! CAN'T UPDATE VERSION! --
echo -----------------------------------------

goto :EOF

:UNEXPECTED_ERROR

echo ------------------------------------------------
echo -- INTERNAL ERROR WHILE RECOMPILING! EXITING! --
echo ------------------------------------------------

goto :EOF
