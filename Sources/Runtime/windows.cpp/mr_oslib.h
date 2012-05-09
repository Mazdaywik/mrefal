#ifndef MR_OSLib_H_
#define MR_OSLib_H_

#include "refalrts.h"
#include "refalmacro.h"

namespace implement_os {

extern REFAL_FUNC(command_line);
extern REFAL_FUNC(env_list);
extern REFAL_FUNC(create_process);

} // namespace implement_os

namespace implement_fs {

extern REFAL_FUNC(find_files);
extern REFAL_FUNC(file_attributes);
extern REFAL_FUNC(full_name);

typedef bool (*FileFunc)(const char *file_name);

extern bool make_dir(const char *file_name);
extern bool remove_dir(const char *file_name);
extern bool remove_file(const char *file_name);

refalrts::FnResult perform_file_operation(
  FileFunc operation,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
);

} // namespace implement_fs

#endif //MR_OSLib_H_
