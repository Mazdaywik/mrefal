@setlocal
  @..\..\..\simple-refal-distrib\bin\srefc-core _mrefalrts.sref
  @move _mrefalrts.native.cpp mrefalrts.cpp
  :: TODO: fix for next versions of Simple Refal
  @erase _mrefalrts.cpp
@endlocal
