#include <stdio.h>
#include "refalrts.h"
#include "mr_oslib.h"


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

//End of file
