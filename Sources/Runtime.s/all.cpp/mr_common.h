#ifndef MR_Common_H_
#define MR_Common_H_

#include "refalrts.h"
#include "refalmacro.h"

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
get_stdin(refalrts::Iter arg_begin, refalrts::Iter arg_end);

refalrts::FnResult
get_stdout(refalrts::Iter arg_begin, refalrts::Iter arg_end);

} // namespace implement_fileio

namespace implement_strings {

typedef void (*SymConverter)(refalrts::Iter i);

void chr(refalrts::Iter i);
void ord(refalrts::Iter i);

refalrts::FnResult
convert(SymConverter conv, refalrts::Iter arg_begin, refalrts::Iter arg_end);

extern REFAL_FUNC(symb);
extern REFAL_FUNC(numb);

extern REFAL_FUNC(serialize_atom);

} // namespace implement_strings

// Эта функция генерится автоматически вместе с Go ...
extern REFAL_FUNC(ExitE_);
// ... и вызывает эту функцию
extern REFAL_FUNC(Exit);

namespace implement_order {

extern REFAL_FUNC(symb_compare);

} // namespace implement_order

namespace implement_selfdiag {

extern refalrts::FnResult log(
  void *f,
  bool transparent,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
);

extern REFAL_FUNC(exit_failure);
extern REFAL_FUNC(close_log);
extern void* get_log_handle();
extern void* get_stderror_handle();

} // namespace implement_selfdiag

#endif //MR_Common_H_
