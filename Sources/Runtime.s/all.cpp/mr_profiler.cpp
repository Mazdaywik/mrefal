#include <cstdio>
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
  bool function_matched =
    refalrts::svar_left(function_name, arg_begin, arg_end)
    && (refalrts::cDataFunction == function_name->tag);

  refalrts::Iter profiler_object = 0;
  bool object_matched =
    refalrts::svar_left(profiler_object, arg_begin, arg_end)
    && (refalrts::cDataFile == profiler_object->tag);

  bool matched =
    function_matched && object_matched &&
    refalrts::empty_seq(arg_begin, arg_end);

  if (!matched)
    return refalrts::cRecognitionImpossible;

  // Останавливаем профилировку
  Profiler *profiler =
    reinterpret_cast<Profiler*>(profiler_object->file_info);
  Profiler::destroy(profiler);

  refalrts::reset_allocator();
  refalrts::splice_to_freelist(open_call, close_call);
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

  // Аргумент: <Quantify s.Func e.Args>
  // Результат: <s.Fucn e.Args> <end_quantify s.Func s.ProfilerObject>

  // Запуск профилировщика осуществляется конструктором класса
  // Profiler, остановка — деструктором. В этой функции создаётся
  // объект класса Profiler, который уничтожается в end_quantify.

  // Стратегия:
  //   1. Распределяем ><end_quantify s.Func s.ProfilerObject
  //   2. Помещаем распределённое в п.1. перед закрывающей
  //      угловой скобкой.
  //   3. Удаляем вызов Quantify

  // Распределение памяти
  refalrts::Iter new_close_call = 0;
  refalrts::Iter new_open_call = 0;
  refalrts::Iter end_quantify_func = 0;
  refalrts::Iter copy_func = 0;
  refalrts::Iter profiler_object = 0;

  refalrts::reset_allocator();

  bool allocated =
    refalrts::alloc_close_call(new_close_call)
    && refalrts::alloc_open_call(new_open_call)
    && refalrts::alloc_name(
        end_quantify_func, end_quantify,
#ifdef MODULE_REFAL
        REFAL_IDENT(endQualityName)
#else
        PROFILE_STOP_FUNCTION_NAME
#endif
       )
    && refalrts::copy_stvar(copy_func, func)
    && refalrts::alloc_char(profiler_object, '?');

  refalrts::Iter func_open_call = open_call;
  refalrts::Iter func_close_call = new_close_call;
  refalrts::Iter stop_open_call = new_open_call;
  refalrts::Iter stop_close_call = close_call;

  if ( ! allocated )
    return refalrts::cNoMemory;

  const char *func_name =
#ifdef MODULE_REFAL
    (func->function_info.name)();
#else
    func->function_info.name;
#endif

  profiler_object->char_info = 0;
  profiler_object->tag = refalrts::cDataFile;
  profiler_object->file_info = Profiler::create(func_name);

  // Перемещаем аллоцированный кусок перед закрывающей скобкой
  refalrts::splice_evar(close_call, new_close_call, profiler_object);

  // Удаляем вызов Quantify
  refalrts::splice_to_freelist(this_func_name, this_func_name);

  refalrts::push_stack(stop_close_call);
  refalrts::push_stack(stop_open_call);
  refalrts::push_stack(func_close_call);
  refalrts::push_stack(func_open_call);

  return refalrts::cSuccess;
}

REFAL_FUNC(implement_profiler::print_results)
{
  refalrts::Iter open_call = arg_begin;
  refalrts::Iter close_call = arg_end;
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_right( arg_begin, arg_end );

  if (refalrts::empty_seq(arg_begin, arg_end))
  {
    using namespace std;
    Profiler::OutToStream(stdout);
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

    if (std::FILE *fout = std::fopen(file_name.c_str(), "wt")) {
      Profiler::OutToStream(fout);
      std::fclose(fout);
    }
  }

  refalrts::reset_allocator();
  refalrts::splice_to_freelist( open_call, close_call );
  return refalrts::cSuccess;
}

REFAL_FUNC(implement_profiler::set_output_name) {
  refalrts::Iter open_call = arg_begin;
  refalrts::Iter close_call = arg_end;
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_right( arg_begin, arg_end );

  CharArray file_name;
  refalrts::FnResult match_filename =
    CharArray::from_sequence(file_name, arg_begin, arg_end);

  if (refalrts::cSuccess != match_filename) {
    return refalrts::cRecognitionImpossible;
  }

  Profiler::set_output_name(file_name.c_str());

  refalrts::reset_allocator();
  refalrts::splice_to_freelist(open_call, close_call);
  return refalrts::cSuccess;
}

REFAL_FUNC(implement_profiler::flush) {
  refalrts::Iter open_call = arg_begin;
  refalrts::Iter close_call = arg_end;
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_left( arg_begin, arg_end );
  refalrts::move_right( arg_begin, arg_end );

  if (! refalrts::empty_seq(arg_begin, arg_end)) {
    return refalrts::cRecognitionImpossible;
  }

  Profiler::flush();

  refalrts::reset_allocator();
  refalrts::splice_to_freelist(open_call, close_call);
  return refalrts::cSuccess;
}
