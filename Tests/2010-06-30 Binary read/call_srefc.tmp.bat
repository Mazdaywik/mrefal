@call compile_sr_cl.bat "Test.main.sref" "~SimRef-01\Test.sref" "..\..\Sources\Libraries\all.cppsr01-s\~SimRef-01\InOut.sref" "..\..\Sources\Libraries\windows.all\~SimRef-01\MDosWinRecoder.sref" "~SimRef-01\Binary.sref" "..\..\Sources\Libraries\all.cppsr01-b\~SimRef-01\Core\MFileIO.sref" 
@del *.obj
@rem del %0
