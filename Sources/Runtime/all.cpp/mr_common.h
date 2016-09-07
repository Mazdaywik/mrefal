#ifndef MR_Common_H_
#define MR_Common_H_

#include "refalrts.h"
#include "refalmacro.h"

namespace implement_math {

extern REFAL_FUNC_IMPL(binary);
extern REFAL_FUNC_IMPL(compare);

} // namespace implement_math

namespace implement_fileio {

extern REFAL_FUNC_IMPL(open);
extern REFAL_FUNC_IMPL(close);
extern REFAL_FUNC_IMPL(read_line);
extern REFAL_FUNC_IMPL(write);

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

extern REFAL_FUNC_IMPL(symb);
extern REFAL_FUNC_IMPL(numb);

extern REFAL_FUNC_IMPL(serialize_atom);

} // namespace implement_strings

// Эта функция генерится автоматически вместе с Go ...
extern refalrts::RefalFunction& ExitE_;
// ... и вызывает эту функцию
extern refalrts::RefalFunction& Exit;

namespace implement_order {

extern REFAL_FUNC_IMPL(symb_compare);

} // namespace implement_order

namespace implement_selfdiag {

extern refalrts::FnResult log(
  void *f,
  bool transparent,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
);

extern REFAL_FUNC_IMPL(exit_failure);
extern REFAL_FUNC_IMPL(close_log);
extern void* get_log_handle();
extern void* get_stderror_handle();

} // namespace implement_selfdiag

#endif //MR_Common_H_
