#include <stdio.h>
#include <assert.h>

#include "refalrts.h"
#include "refalapi.h"
#include "mr_common.h"

/*===========================================================================
  Реализация функций библиотеки.
===========================================================================*/

REFAL_FUNC_IMPL(func_Exit)
{
  // Формат <Exit s.RetCode> == поле зрения не изменяет

  using namespace refalrts;

  // Скобки вызова и имя функции
  // Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  // Разбор образца

  Iter sRetCode = 0;
  bool match =
    svar_left( sRetCode, arg_begin, arg_end )
    && ( cDataNumber == sRetCode->tag )
    && empty_seq( arg_begin, arg_end );

  if( match == false ) return cRecognitionImpossible;

  // Устанавливаем результат возврата
  ::refalrts::set_return_code( sRetCode->number_info );

  // Поле зрения не очищаем
  // Возвращаем сигнал о нормальном останове
  return ::refalrts::cExit;
}

refalrts::RefalNativeFunction descr_Exit(
  func_Exit, refalrts::RefalFuncName("Exit", 0, 0)
);
refalrts::ExternalReference ref_Exit("Exit", 0, 0);
