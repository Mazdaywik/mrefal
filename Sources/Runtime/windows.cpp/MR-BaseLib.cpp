#include "refalrts.h"
#include "baselib.h"


refalrts::FnResult MR_Math_Binary(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_math::binary(arg_begin, arg_end);
}

refalrts::FnResult MR_Math_Compare(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_math::compare(arg_begin, arg_end);
}

refalrts::FnResult MR_IO_Open(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::open(arg_begin, arg_end);
}

refalrts::FnResult MR_IO_Close(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::close(arg_begin, arg_end);
}

refalrts::FnResult MR_IO_ReadLine(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::read_line(arg_begin, arg_end);
}

refalrts::FnResult MR_IO_Write(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::write(arg_begin, arg_end);
}

refalrts::FnResult MR_IO_StdIn(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::std_handle(stdin, arg_begin, arg_end);
}

refalrts::FnResult MR_IO_StdOut(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::std_handle(stdout, arg_begin, arg_end);
}

refalrts::FnResult MR_Strings_Chr(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::convert(
    implement_strings::chr, arg_begin, arg_end
  );
}

refalrts::FnResult MR_Strings_Ord(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::convert(
    implement_strings::ord, arg_begin, arg_end
  );
}

refalrts::FnResult MR_Strings_Symb(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::symb(arg_begin, arg_end);
}

refalrts::FnResult MR_Strings_Numb(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::numb(arg_begin, arg_end);
}

refalrts::FnResult MR_OS_CommandLine(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_os::command_line(arg_begin, arg_end);
}

refalrts::FnResult MR_OS_EnvList(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_os::env_list(arg_begin, arg_end);
}

refalrts::FnResult MR_OS_CreateProcess(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_os::create_process(arg_begin, arg_end);
}

refalrts::FnResult MR_FS_FindFiles(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::find_files(arg_begin, arg_end);
}

refalrts::FnResult MR_FS_FileAttributes(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::file_attributes(arg_begin, arg_end);
}

refalrts::FnResult MR_FS_FullName(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::full_name(arg_begin, arg_end);
}

refalrts::FnResult MR_FS_MakeDir(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::perform_file_operation(
    implement_fs::make_dir, arg_begin, arg_end
  );
}

refalrts::FnResult MR_FS_RemoveDir(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::perform_file_operation(
    implement_fs::remove_dir, arg_begin, arg_end
  );
}

refalrts::FnResult MR_FS_RemoveFile(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::perform_file_operation(
    implement_fs::remove_file, arg_begin, arg_end
  );
}

refalrts::FnResult MR_Order_SymbCompare(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_order::symb_compare(arg_begin, arg_end);
}

refalrts::FnResult MR_SelfDiag_Log(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::log(
    implement_selfdiag::get_log_handle(),
    false, // Непрозрачный режим
    arg_begin, arg_end
  );
}

refalrts::FnResult MR_SelfDiag_Log_T(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::log(
    implement_selfdiag::get_log_handle(),
    true, // Прозрачный режим
    arg_begin, arg_end
  );
}

refalrts::FnResult MR_SelfDiag_PrintMessage(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::log(
    stderr, false, arg_begin, arg_end
  );
}

refalrts::FnResult MR_SelfDiag_ExitFailure(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::exit_failure(arg_begin, arg_end);
}

refalrts::FnResult MR_SelfDiag_CloseLog(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::close_log(arg_begin, arg_end);
}


//End of file
