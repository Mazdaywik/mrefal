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
  refalrts::RefalNativeFunction descr_ ## func(func_ ## func, name); \
  refalrts::RefalFunction& func ## _0_0 = descr_ ## func; \
  static REFAL_FUNC_IMPL(func_ ## func)

/*
  Макрос для объявления идентификаторов.
*/
#define DECL_REFAL_IDENT(ident_name, textual) \
  template <typename T> \
  struct ident_ ## ident_name { \
    static const char *name() { \
      return textual; \
    } \
  };

/*
  Макрос для использования идентификаторов.
*/
#define REFAL_IDENT(ident_name) ident_ ## ident_name <int>::name


#endif //RefalMacro_H_
