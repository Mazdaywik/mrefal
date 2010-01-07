#ifndef Baselib_H_
#define Baselib_H_

#include <stdio.h>

#include "refalrts.h"

/*
  Макрос для написания заголовков рефал-функций.
*/
#define REFAL_FUNC(func) \
  refalrts::FnResult func(refalrts::Iter arg_begin, refalrts::Iter arg_end)

/*
  Макрос для объявления идентификаторов.
*/
#define DECL_REFAL_IDENT(ident_name, textual) \
  template <typename T> \
  struct ident_name ## L_ { \
    static const char *name() { \
      return textual; \
    } \
  };

/*
  Макрос для использования идентификаторов.
*/
#define REFAL_IDENT(ident_name) ident_name ## L_<int>::name

namespace implement_math {

extern REFAL_FUNC(binary);
extern REFAL_FUNC(compare);

} // namespace implement_math

namespace implement_fileio {

extern REFAL_FUNC(open);
extern REFAL_FUNC(close);
extern REFAL_FUNC(read_line);
extern REFAL_FUNC(write);

refalrts::FnResult
std_handle(FILE *handle, refalrts::Iter arg_begin, refalrts::Iter arg_end);

} // namespace implement_fileio

namespace implement_strings {

typedef void (*SymConverter)(refalrts::Iter i);

void chr(refalrts::Iter i);
void ord(refalrts::Iter i);

refalrts::FnResult
convert(SymConverter conv, refalrts::Iter arg_begin, refalrts::Iter arg_end);

extern REFAL_FUNC(symb);
extern REFAL_FUNC(numb);

} // namespace implement_strings

// Эта функция генерится автоматически вместе с Go ...
extern REFAL_FUNC(ExitE_);
// ... и вызывает эту функцию
extern REFAL_FUNC(Exit);

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

namespace implement_order {

extern REFAL_FUNC(symb_compare);

}

namespace implement_selfdiag {

extern refalrts::FnResult log(
  FILE *f,
  bool transparent,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
);

extern REFAL_FUNC(exit_failure);
extern REFAL_FUNC(close_log);
extern FILE* get_log_handle();

} // namespace implement_selfdiag

#endif //Baselib_H_
