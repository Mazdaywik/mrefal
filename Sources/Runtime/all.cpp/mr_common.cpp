#include <stdio.h>
#include <assert.h>

#include "refalrts.h"
#include "refalapi.h"
#include "mr_common.h"

/*===========================================================================
  Реализация функций библиотеки.
===========================================================================*/


refalrts::FnResult implement_strings::convert(
  implement_strings::SymConverter conv,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  // Функция преобразует все узлы внутри аргумента с помощью функции conv

  using namespace refalrts;

  // Скобки конкретизации, имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  for(
    Iter ptr = arg_begin, limit = arg_end;
    ! empty_seq(ptr, limit);
    move_left(ptr, limit)
  ) {
    conv( ptr );
  }

  // Очистка поля зрения
  splice_to_freelist(open_call, func_name);
  splice_to_freelist(close_call, close_call);

  return cSuccess;
}

void implement_strings::chr(refalrts::Iter ptr) {
  using namespace refalrts;

  if( cDataNumber == ptr->tag ) {
    RefalNumber code = ptr->number_info;
    ptr->tag = cDataChar;
    ptr->char_info = static_cast<char>( code );
  } else {
    // Ничего не делаем
  }
}

void implement_strings::ord(refalrts::Iter ptr) {
  using namespace refalrts;

  if( cDataChar == ptr->tag ) {
    unsigned char value = ptr->char_info;
    ptr->tag = cDataNumber;
    ptr->number_info = static_cast<RefalNumber>( value );
  } else {
    // Ничего не делаем
  }
}

REFAL_FUNC_IMPL(implement_strings::symb) {
  /*
    Функция преобразует число в строку
    <Symb s.Number> == e.Text

    Изначально функция Symb в Рефале 5 использовалась
    для преобразования в строку цепочки макроцифр
    и, соответственно, принимала в качестве аргумента
    цепочку макроцифр, но т.к. Модульный Рефал макроцифр
    не поддерживает, было решено сократить формат
    до подмножества --- чисел из одной макроцифры.
  */

  using namespace refalrts;

  // Скобки вызова, имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  Iter sNumber = 0;
  bool match_number =
    svar_left( sNumber, arg_begin, arg_end )
    && ( cDataNumber == sNumber->tag )
    && empty_seq( arg_begin, arg_end );

  if( match_number == false ) return cRecognitionImpossible;

  // На один байт уходит примерно три десятичных разряда.
  enum { cMaxLen = 3 * sizeof(RefalNumber) };
  char buffer[ cMaxLen + 1 ] = { 0 };
  Iter number_b = 0, number_e = 0;
  char *last_pos = buffer + cMaxLen;
  RefalNumber num = sNumber->number_info;

  /*
    Сей хитрый цикл успешно обрабатывает случай, когда num == 0.
  */
  do {
    RefalNumber last_digit = num % 10;
    num /= 10;
    --last_pos;
    *last_pos = static_cast<char>(last_digit + '0');
  } while( num != 0 );

  reset_allocator();

  if( ! alloc_string(number_b, number_e, last_pos) )
    return cNoMemory;

  splice_evar( open_call, number_b, number_e );
  splice_to_freelist( open_call, close_call );
  return cSuccess;
}

REFAL_FUNC_IMPL(implement_strings::numb) {
  /*
    Функция преобразует цепочку символов в число.

    <Numb e.Numbers e.Rest> == s.Number

    Преобразованию подвергаются только числа, идущие в начале аргумента,
    последующая часть строки игнорируется.
    Если e.Numbers представляют собой десятичное число,
    большее максимального значения RefalNumber,
    результат функции не определён.
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  RefalNumber result = 0;

  for(
    Iter ptr = arg_begin, limit = arg_end;
    ! empty_seq( ptr, limit ) && (cDataChar == ptr->tag )
    && ('0' <= ptr->char_info) && (ptr->char_info <= '9');
    move_left( ptr, limit )
  ) {
    result *= 10;
    result += (ptr->char_info - '0');
  }

  // Повторно используем открывающую угловую скобку
  open_call->link_info = 0;
  open_call->tag = cDataNumber;
  open_call->number_info = result;
  // Удаляем Numb e.Numbers e.Rest>
  splice_to_freelist( func_name, close_call );

  return cSuccess;
}

DECL_REFAL_IDENT(Char, "Char");
DECL_REFAL_IDENT(Number, "Number");
DECL_REFAL_IDENT(IsntSerializable, "IsntSerializable");

namespace {

refalrts::RefalNativeFunction symb_descr(
  implement_strings::symb, refalrts::RefalFuncName("Symb", 0, 0)
);

}

REFAL_FUNC_IMPL(implement_strings::serialize_atom) {
  // Формат
  // <SerializeAtom s.Atom>
  //   == Char e.CharRep
  //   == Number e.StrNumber
  //   == IsntSerializable s.Atom
  // e.CharRep ::= s.Atom | s.Digit1 s.Digit2 s.Digit3
  // e.StrNumber — десятичная запись числа

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
   Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  Iter sAtom = 0;
  bool pattern =
    svar_left(sAtom, arg_begin, arg_end)
    && empty_seq(arg_begin, arg_end);

  if( pattern == false ) return cRecognitionImpossible;

  if( cDataChar == sAtom->tag ) {
    if( (32 <= sAtom->char_info) && (sAtom->char_info < 127) ) {
      func_name->tag = cDataIdentifier;
      func_name->ident_info = REFAL_IDENT(Char);

      splice_to_freelist(open_call, open_call);
      splice_to_freelist(close_call, close_call);

      return cSuccess;
    } else {
      open_call->tag = cDataIdentifier;
      open_call->ident_info = REFAL_IDENT(Char);

      unsigned code = static_cast<unsigned char>(sAtom->char_info);

      func_name->tag = cDataChar;
      func_name->char_info = (code / 100 % 10) + '0';

      sAtom->char_info = (code / 10 % 10) + '0';

      close_call->tag = cDataChar;
      close_call->char_info = (code % 10) + '0';

      return cSuccess;
    }
  } else if( cDataNumber == sAtom->tag ) {
    Iter number_ident = 0;
    if( ! alloc_ident(number_ident, REFAL_IDENT(Number)) )
      return cNoMemory;

    splice_stvar(open_call, number_ident);

    // Необычный ход для не-рефальной функции:
    // формирование в результатном выражении вызова другой функции
    func_name->function_info = &symb_descr;
    push_stack(close_call);
    push_stack(open_call);

    return cSuccess;
  } else {
      func_name->tag = cDataIdentifier;
      func_name->ident_info = REFAL_IDENT(IsntSerializable);

      splice_to_freelist(open_call, open_call);
      splice_to_freelist(close_call, close_call);

      return cSuccess;
  }
}

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

refalrts::FnResult implement_selfdiag::log(
  void *flog,
  bool transparent,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  /*
    Данная функция вызывается из функций печати логирующих сообщений.
    Печатает в лог информацию в том же формате, что и при аварийном
    дампе поля зрения.
    Параметр flog задаёт поток, в который будет писаться дамп ---
    как правило или в файл __log или на stderr.
    Параметр transparent задаёт режим сохранения аргумента:
      * true --- аргумент в поле зрения сохраняется, удаляются только
        скобки конкретизации и имя функции;
      * false --- поле зрения после завершения функции оказывается
        пустым.

    Формат цепочки arg_begin..arg_end:
      <Log e.Message> == пусто
      <Log-T e.Message> == e.Message

    Если flog == 0, то вывод производится на stderr.
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left( arg_begin, arg_end );
  Iter func_name = arg_begin;
  move_left( arg_begin, arg_end );
  Iter close_call = arg_end;
  move_right( arg_begin, arg_end );

  FILE *fileLOG = (FILE*) flog;

  // Вывод дампа:
  if( (0 == fileLOG) || (stderr == fileLOG) ) {
    fprintf(stderr, "<Log>: ");
    fileLOG = stderr;
  }

  refalrts::debug_print_expr(fileLOG, arg_begin, arg_end);
  fprintf(fileLOG, "\n");
  fflush(fileLOG);

  // Очистка поля зрения
  if( transparent ) {
    splice_to_freelist( open_call, func_name );
    splice_to_freelist( close_call, close_call );
  } else {
    splice_to_freelist( open_call, close_call );
  }

  return cSuccess;
}

REFAL_FUNC_IMPL(implement_selfdiag::exit_failure) {
  /*
    Предполагается, что функция будет вызывать все финализаторы,
    поэтому вызывает функцию ExitE_.
  */

  using namespace refalrts;

  // Понятно, что
  // Iter open_call = arg_begin;
  move_left( arg_begin, arg_end );
  Iter func_name = arg_begin;
  move_left( arg_begin, arg_end );
  Iter close_call = arg_begin;
  move_right( arg_begin, arg_end );

  if( ! empty_seq(arg_begin, arg_end) ) return cRecognitionImpossible;

  Iter sRetValue = 0;

  if( ! alloc_number(sRetValue, 255) ) return cNoMemory;

  // Переинициализация имени функции
  func_name->function_info = ref_Exit.ref.function;

  // Вставка кода возврата после имени функции
  splice_elem( close_call, sRetValue );

  return cSuccess;
}

namespace {

// Глобальный дескриптор файла лога
FILE *g_log_file = 0;

} // безымянное namespace

REFAL_FUNC_IMPL(implement_selfdiag::close_log) {
  /*
    Закрывает файл лога.

    Формат:
      <CloseLog> == пусто.
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_begin;
  move_right(arg_begin, arg_end);

  if( ! empty_seq(arg_begin, arg_end) ) return cRecognitionImpossible;

  if( 0 != g_log_file ) {
    fclose( g_log_file );
  }

  splice_to_freelist(open_call, close_call);
  return cSuccess;
}

void *implement_selfdiag::get_log_handle() {
  if( 0 == g_log_file ) {
    g_log_file = fopen("__log", "wt");
  }

  return g_log_file;
}

void *implement_selfdiag::get_stderror_handle() {
  return stderr;
}

