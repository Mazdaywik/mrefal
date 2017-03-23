#ifndef RefalMacro_H_
#define RefalMacro_H_

#include "refalrts.h"

/*
  Макрос для написания заголовков рефал-функций.
*/
#define REFAL_FUNC_IMPL(func) \
  refalrts::FnResult func(refalrts::Iter arg_begin, refalrts::Iter arg_end)

#define REFAL_FUNC(func, name) \
  static REFAL_FUNC_IMPL(func_ ## func); \
  refalrts::RefalNativeFunction descr_ ## func( \
    func_ ## func, refalrts::RefalFuncName(name, 0, 0) \
  ); \
  static REFAL_FUNC_IMPL(func_ ## func)

/*
  Макрос для объявления идентификаторов.
*/
#define DECL_REFAL_IDENT(ident_name, textual) \
  const refalrts::RefalIdentifier ident_ ## ident_name = \
    refalrts::ident_from_static(textual);

/*
  Макрос для использования идентификаторов.
*/
#define REFAL_IDENT(ident_name) ident_ ## ident_name


#endif //RefalMacro_H_
