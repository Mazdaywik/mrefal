#ifndef RefalMacro_H_
#define RefalMacro_H_

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
  struct ident_name { \
    static const char *name() { \
      return textual; \
    } \
  };

/*
  Макрос для использования идентификаторов.
*/
#define REFAL_IDENT(ident_name) ident_name <int>::name

#endif //RefalMacro_H_
