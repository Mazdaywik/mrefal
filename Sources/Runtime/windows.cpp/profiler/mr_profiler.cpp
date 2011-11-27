#include <iostream>
#include <fstream>
#include "refalrts.h"
#include "refalapi.h"
#include "mr_profiler.h"

#include "Profiler.h"

#define PROFILE_STOP_FUNCTION_NAME "@StopProfilerFunction"

using refalapi::CharArray;
using profiler::Profiler;

namespace {

REFAL_FUNC(end_quantify)
{
  refalrts::Iter open_call = arg_begin;
  refalrts::Iter close_call = arg_end;

  refalrts::move_left(arg_begin, arg_end);
  refalrts::move_left(arg_begin, arg_end);
  refalrts::move_right(arg_begin, arg_end);

  // Образец: <end_quantify s.Function s.ProfilerObject>
  refalrts::Iter function_name = 0;
  bool function_mathed =
    refalrts::svar_left(function_name, arg_begin, arg_end)
    && (refalrts::cDataFunction == function_name->tag);

  refalrts::Iter profiler_object = 0;
  bool object_matched =
    refalrts::svar_left(profiler_object, arg_begin, arg_end)
    && (refalrts::cDataFile == profiler_object->tag);

  bool matched =
    function_mathed && object_matched &&
    refalrts::empty_seq(arg_begin, arg_end);

  if (!matched)
    return refalrts::cRecognitionImpossible;

  // Останавливаем профилировку
  Profiler *profiler =
    reinterpret_cast<Profiler*>(profiler_object->file_info);
  delete profiler;

  refalrts::reset_allocator();
  splice_to_freelist(open_call, close_call);
  return refalrts::cSuccess;
}

DECL_REFAL_IDENT(endQualityName, PROFILE_STOP_FUNCTION_NAME);

} // безымянное namespace

REFAL_FUNC(implement_profiler::quantify)
{
  // Угловые скобки, имя функции
  refalrts::Iter open_call = arg_begin;
  refalrts::move_left(arg_begin, arg_end);

  refalrts::Iter this_func_name = arg_begin;
  refalrts::move_left(arg_begin, arg_end);

  refalrts::Iter close_call = arg_end;
  refalrts::move_right(arg_begin, arg_end);

  // Образец: s.Func e.Args
  refalrts::Iter func = 0;
  bool matched =
    refalrts::svar_left( func, arg_begin, arg_end )
    && (refalrts::cDataFunction == func->tag);
  if( ! matched )
    return refalrts::cRecognitionImpossible;

  //refalrts::Iter func_args_b = arg_begin; // не используется
  refalrts::Iter func_args_e = arg_end;

  // Результат: <s.Fucn e.Args> <end_quantify s.Func s.ProfilerObject>

  // Запуск профилировщика осуществляется конструктором класса
  // Profiler, остановка — деструктором. В этой функции создаётся
  // объект класса Profiler, который уничтожается в end_quantify.

  // Стратегия: распределяем < и > для s.Func e.Args, остальные
  // элементы размещаем между родными угловыми скобками.

  // Распределение памяти
  refalrts::Iter new_open_call = 0;
  refalrts::Iter new_close_call = 0;

  refalrts::Iter copy_func = 0;
  refalrts::Iter profiler_object = 0;

  refalrts::reset_allocator();

  bool allocated =
    refalrts::alloc_open_call(new_open_call)
    && refalrts::alloc_close_call(new_close_call)
    && refalrts::copy_stvar(copy_func, func)
    && refalrts::alloc_char(profiler_object, '?');

  if ( ! allocated )
    return refalrts::cNoMemory;

  // Переинициализация имени функции
  this_func_name->function_info.ptr = end_quantify;
  this_func_name->function_info.name =
#ifdef MODULE_REFAL
    REFAL_IDENT(endQualityName);
#else
    PROFILE_STOP_FUNCTION_NAME;
#endif

  const char *func_name =
#ifdef MODULE_REFAL
    (func->function_info.name)();
#else
    func->function_info.name;
#endif

  profiler_object->char_info = 0;
  profiler_object->tag = refalrts::cDataFile;
  profiler_object->file_info = new Profiler(func_name);

  // Сборка поля зрения
  refalrts::Iter res = open_call;
  res = refalrts::splice_elem(res, new_close_call);
  // Пользуемся тем, что func и func_args_? идут последовательно
  res = refalrts::splice_evar(res, func, func_args_e);
  refalrts::splice_elem(res, new_open_call);

  res = close_call;
  res = refalrts::splice_elem(res, profiler_object);
  refalrts::splice_elem(res, copy_func);

  refalrts::push_stack(close_call);
  refalrts::push_stack(open_call);
  refalrts::push_stack(new_close_call);
  refalrts::push_stack(new_open_call);

  return refalrts::cSuccess;
}

REFAL_FUNC(implement_profiler::print_results)
{
  refalrts::Iter open_call = arg_begin;
  refalrts::Iter close_call = arg_end;
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_right( arg_begin, arg_end );

  if (empty_seq(arg_begin, arg_end))
  {
    Profiler::OutToStream(std::cout);
  }
  else
  {
    CharArray file_name;
    refalrts::FnResult match_filename =
      CharArray::from_sequence(file_name, arg_begin, arg_end);

    if (refalrts::cSuccess != match_filename)
    {
      return refalrts::cRecognitionImpossible;
    }

    std::ofstream fout(file_name.c_str());
    Profiler::OutToStream(fout);
  }

  refalrts::reset_allocator();
  refalrts::splice_to_freelist( open_call, close_call );
  return refalrts::cSuccess;
}


