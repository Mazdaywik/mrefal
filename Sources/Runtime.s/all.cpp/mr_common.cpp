#include <stdio.h>
#include <assert.h>

#include "refalrts.h"
#include "refalapi.h"
#include "mr_common.h"

using refalapi::CharArray;
using refalapi::AllocArray;

/*===========================================================================
  Реализация функций библиотеки.
===========================================================================*/

REFAL_FUNC(implement_math::binary) {
  using namespace refalrts;

  // Левая скобка конкретизации
  Iter left_call = arg_begin;
  move_left(arg_begin, arg_end);

  // Имя функции
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);

  // Правая скобка конкретизации
  Iter right_call = arg_end;
  move_right(arg_begin, arg_end);

  /*
    Образец s.Op s.X s.Y,
    s.Op --- операция, одно из '+-*%/',
    s.X, s.Y --- целые числа.
  */
  Iter sOp = 0, sX = 0, sY = 0;

  bool pattern =
    svar_left( sOp, arg_begin, arg_end ) && ( cDataChar == sOp->tag )
    && svar_left( sX, arg_begin, arg_end ) && ( cDataNumber == sX->tag )
    && svar_left( sY, arg_begin, arg_end ) && ( cDataNumber == sY->tag )
    && empty_seq( arg_begin, arg_end );

  if( pattern == false ) return cRecognitionImpossible;

  RefalNumber x = sX->number_info;
  RefalNumber y = sY->number_info;

  // Вычисление арифметической операции

  switch( sOp->char_info ) {
  case '+':
    x = x + y;
    break;

  case '-':
    x = x - y;
    break;

  case '*':
    x = x * y;
    break;

  case '/':
  case '%':
    if( y != 0 ) {
      x = ('/' == sOp->char_info) ? (x / y) : (x % y);
      break;
    } else {
      return cRecognitionImpossible;
    }
  default:
    return cRecognitionImpossible;
  }

  // Построение результата

  sX->number_info = x;

  // Уничтожение <Func s.Op
  splice_to_freelist(left_call, sOp);
  // Уничтожение s.Y>
  splice_to_freelist(sY, right_call);

  return cSuccess;
}

REFAL_FUNC(implement_math::compare) {
  // <Math::Compare s.X s.Y>

  using namespace refalrts;

  // Левая скобка конкретизации
  Iter left_call = arg_begin;
  move_left(arg_begin, arg_end);

  // Правая скобка конкретизации
  Iter right_call = arg_end;
  move_right(arg_begin, arg_end);

  // Имя функции
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);

  // Сопоставление с образцом
  Iter sX = 0, sY = 0;

  bool pattern =
    svar_left( sX, arg_begin, arg_end ) && ( cDataNumber == sX->tag )
    && svar_left( sY, arg_begin, arg_end ) && ( cDataNumber == sY->tag )
    && empty_seq( arg_begin, arg_end );

  if( pattern == false ) return cRecognitionImpossible;

  RefalNumber x = sX->number_info;
  RefalNumber y = sY->number_info;

  // Переинициализация открывающей скобки
  left_call->link_info = 0;
  left_call->tag = cDataChar;
  left_call->char_info = (x < y) ? '<' : (x == y) ? '=' : '>';

  // Удаление лишнего: Math::Compare s.X s.Y>
  splice_to_freelist(func_name, right_call);

  return cSuccess;
}

DECL_REFAL_IDENT(Fails, "Fails");

REFAL_FUNC(implement_fileio::open) {
  using namespace refalrts;

  // Образец: <Open ['b'] s.Mode e.FileName>

  // Левая и правая скобки, имя функции
  Iter left_call = arg_begin;
  move_left(arg_begin, arg_end);

  Iter right_call = arg_end;
  move_right(arg_begin, arg_end);

  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);

  Iter sMode = 0;

  bool mode_parsed =
    svar_left( sMode, arg_begin, arg_end ) && ( cDataChar == sMode->tag );

  if( mode_parsed == false ) return cRecognitionImpossible;

  char ch_mode = sMode->char_info;
  bool binary = ('b' == ch_mode);

  if( binary )
  {
    mode_parsed =
      svar_left( sMode, arg_begin, arg_end )
      && ( cDataChar == sMode->tag );

    if( mode_parsed == false ) return cRecognitionImpossible;
  }

  ch_mode = sMode->char_info;
  const char *str_mode =
    ('r' == ch_mode) ? (binary ? "rb" : "rt") :
    ('w' == ch_mode) ? (binary ? "wb" : "wt") :
    ('a' == ch_mode) ? (binary ? "ab" : "at") : 0;

  if( str_mode == 0 ) return cRecognitionImpossible;

  CharArray file_name;

  FnResult name_parse =
    CharArray::from_sequence( file_name, arg_begin, arg_end );

  if( cSuccess != name_parse ) return name_parse;

  if( FILE *f = fopen(file_name.c_str(), str_mode) ) {
    // Переинициализация открывающей угловой скобки
    left_call->link_info = 0;
    left_call->tag = cDataFile;
    left_call->file_info = f;

    // Уничтожение остальных элементов поля зрения
    splice_to_freelist(func_name, right_call);
    return cSuccess;
  } else {
    // Переинициализация открывающей угловой скобки
    left_call->link_info = 0;
    left_call->tag = cDataIdentifier;
    left_call->ident_info = REFAL_IDENT(Fails);

    splice_to_freelist(func_name, right_call);
    return cSuccess;
  }
}

REFAL_FUNC(implement_fileio::close) {
  // Функция закрывает дескриптор файла
  // <Close s.FileHandle> == пусто

  using namespace refalrts;

  // Скобки конкретизации и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);

  Iter sHandle = 0;

  // Сопоставление с образцом
  bool pattern =
    svar_left( sHandle, arg_begin, arg_end )
    && ( cDataFile == sHandle->tag )
    && empty_seq( arg_begin, arg_end );

  if( pattern == false ) return cRecognitionImpossible;

  int fclose_res = fclose( static_cast<FILE*>( sHandle->file_info ) );

  if( EOF != fclose_res ) {
    splice_to_freelist(open_call, close_call);
    return cSuccess;
  } else {
    return cRecognitionImpossible;
  }
}

REFAL_FUNC(implement_fileio::read_line) {
  // Функция считывания строки со стандартного потока
  // <Read s.Handle> == e.Line

  using namespace refalrts;

  // Угловые скобки и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);

  Iter sHandle = 0;
  bool match =
    svar_left( sHandle, arg_begin, arg_end )
    && ( cDataFile == sHandle->tag )
    && empty_seq( arg_begin, arg_end );

  if( match == false ) return cRecognitionImpossible;

  // Считывание строки в массив

  enum { cBufSize = 100 };
  char buffer[cBufSize + 1] = { 0 };
  bool at_eol = false, at_eof = false;
  AllocArray allocs;
  FILE *f = static_cast<FILE*>( sHandle->file_info );

  reset_allocator();

  while( ! at_eol ) {
    unsigned pos = 0;

    while( (pos < cBufSize) && ! at_eol ) {
      int c = fgetc(f);
      if( (EOF == c) || ('\n' == c) ) {
        at_eol = true;
        at_eof = (EOF == c);
        buffer[pos] = '\0';
      } else {
        buffer[pos] = static_cast<char>(c);
        ++pos;
      }
    }

    Iter frag_begin = 0, frag_end = 0;

    if(pos > 0) {
      if( !alloc_chars(frag_begin, frag_end, buffer, pos) ) {
        return cNoMemory;
      }

      allocs.push_back(frag_begin, frag_end);
    }
  }

  if( at_eof ) {
    // Добавляем в конец строки число 0
    Iter sEOFMarker = 0;
    if( ! alloc_number( sEOFMarker, 0 ) ) return cNoMemory;
    allocs.push_back( sEOFMarker );
  }

  allocs.build( open_call );
  splice_to_freelist( open_call, close_call );

  return cSuccess;
}

REFAL_FUNC(implement_fileio::write) {
  // Функция печатает объектное выражение, являющееся аргументом
  // <Write s.Handle e.Expr> == пусто

  using namespace refalrts;

  // Угловые скобки и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  Iter sHandle = 0;
  bool match_handle =
    svar_left( sHandle, arg_begin, arg_end )
    && ( cDataFile == sHandle->tag );

  if( match_handle == false ) return cRecognitionImpossible;

  FILE *f = static_cast<FILE*>( sHandle->file_info );

  if( ferror(f) ) return cRecognitionImpossible;

  int printf_res = 0;

  for(
    Iter ptr = arg_begin, limit = arg_end;
    (printf_res >= 0) && ! empty_seq(ptr, limit);
    move_left(ptr, limit)
  ) {
    switch( ptr->tag ) {
      case cDataChar:
        printf_res = fprintf(f, "%c", ptr->char_info);
        continue;

      case cDataNumber:
        printf_res = fprintf(f, "%ld ", ptr->number_info);
        continue;

      case cDataFunction:
#ifdef MODULE_REFAL
        printf_res = fprintf(f, "%s ", (ptr->function_info.name)());
#else
        printf_res = fprintf(f, "%s ", ptr->function_info.name);
#endif
        continue;

      case cDataIdentifier:
        printf_res = fprintf(f, "%s ", (ptr->ident_info)());
        continue;

      case cDataOpenADT:
        printf_res = fprintf(f, "[");
        continue;

      case cDataCloseADT:
        printf_res = fprintf(f, "]");
        continue;

      case cDataOpenBracket:
        printf_res = fprintf(f, "(");
        continue;

      case cDataCloseBracket:
        printf_res = fprintf(f, ")");
        continue;

      case cDataFile:
        printf_res = fprintf(f, "*%p", ptr->file_info);
        continue;

      default:
        // Неожиданный тип узла в объектном выражении
        assert( SWITCH_VIOLATION );
    }
  }

  // Здесь (printf_res < 0) || empty_seq(ptr, limit)
  if( printf_res >= 0 ) {
    // Успешный вывод в поток
    splice_to_freelist(open_call, close_call);
    return cSuccess;
  } else {
    // В процессе вывода произшла ошибка
    return cRecognitionImpossible;
  }
}

static refalrts::FnResult std_handle(
  FILE *file, refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  /*
    Функция возвращает заданный дескриптор.
    Формат участка arg_begin..arg_end:
      <StdHandle> == s.Handle
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  if( ! empty_seq(arg_begin, arg_end) ) return cRecognitionImpossible;

  // Повторное использование открывающей скобки вызова
  open_call->link_info = 0;
  open_call->tag = cDataFile;
  open_call->file_info = file;

  // Очистка поля зрения
  splice_to_freelist( func_name, close_call );

  return cSuccess;
}

refalrts::FnResult
implement_fileio::get_stdin(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return std_handle(stdin, arg_begin, arg_end);
}

refalrts::FnResult
implement_fileio::get_stdout(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return std_handle(stdout, arg_begin, arg_end);
}


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

REFAL_FUNC(implement_strings::symb) {
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

REFAL_FUNC(implement_strings::numb) {
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
DECL_REFAL_IDENT(Symb, "Symb");

REFAL_FUNC(implement_strings::serialize_atom) {
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
    func_name->function_info.ptr = symb;
#ifdef MODULE_REFAL
    func_name->function_info.name = REFAL_IDENT(Symb);
#else
    func_name->function_info.name = "Symb";
#endif
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

REFAL_FUNC(Exit) {
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

REFAL_FUNC(implement_order::symb_compare) {
  /*
    Функция сравнения двух атомов. Семантика функции устаревшая,
    т.к. наследует ограничения Модульного Рефала,
    основанного на back-end'е Рефала 5.

    Порядок сравнения:
      Числа > Символы > Всё остальное
    Числа сравниваются по значению, символы сравниваются
    по ASCII-кодам, всё остальное принимается равным.

    Формат:
      <SymbCompare s.Left s.Right> == '<' | '=' | '>'
  */

  using namespace refalrts;

  // Угловые скобки, имя функции.
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);

  // Сопоставление с образцом
  Iter sLeft = 0, sRight = 0;
  bool match =
    svar_left( sLeft, arg_begin, arg_end )
    && svar_left( sRight, arg_begin, arg_end )
    && empty_seq( arg_begin, arg_end );

  if( ! match ) return cRecognitionImpossible;

  // Сравнение
  char compare_sign;

  if( sLeft->tag == sRight->tag ) {
    switch( sLeft->tag ) {
      case cDataNumber:
        compare_sign =
          (sLeft->number_info < sRight->number_info) ? '<' :
          (sLeft->number_info == sRight->number_info) ? '=' :
          '>';
        break;

     case cDataChar:
       compare_sign =
         (sLeft->char_info < sRight->char_info) ? '<' :
         (sLeft->char_info == sRight->char_info) ? '=' :
         '>';
       break;

     default:
       compare_sign = '=';
       //break;
    }
  } else {
    compare_sign =
      ( cDataNumber == sLeft->tag ) ? '<' :
      ( cDataNumber == sRight->tag ) ? '>' :
      ( cDataChar == sLeft->tag ) ? '<' :
      ( cDataChar == sRight->tag ) ? '>' :
      '=';
  }

  // Переинициализация открывающей скобки
  open_call->link_info = 0;
  open_call->tag = cDataChar;
  open_call->char_info = compare_sign;

  splice_to_freelist( func_name, close_call );
  return cSuccess;
}

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

DECL_REFAL_IDENT(Exit_FuncNameE_, "@Exit");

REFAL_FUNC(implement_selfdiag::exit_failure) {
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
  func_name->function_info.ptr = ExitE_;
#ifdef MODULE_REFAL
  func_name->function_info.name = REFAL_IDENT(Exit_FuncNameE_);
#else
  func_name->function_info.name = "@Exit";
#endif

  // Вставка кода возврата после имени функции
  splice_elem( close_call, sRetValue );

  return cSuccess;
}

namespace {

// Глобальный дескриптор файла лога
FILE *g_log_file = 0;

} // безымянное namespace

REFAL_FUNC(implement_selfdiag::close_log) {
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

