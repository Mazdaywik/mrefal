#include <vector>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <deque>
#include <utility>
#include <stdlib.h>
#include <assert.h>

#include "refalrts.h"
#include "baselib.h"

/*===========================================================================
  Утилитарные функции для работы с промежуточным представлением поля зрения.
===========================================================================*/

namespace {

/*
   Класс строк, основанный на стандартном векторе. Класс std::string
   не используем, т.к. при работе с WinAPI-функциями нам требуется
   прямой доступ к си-массиву, который не может предоставить нам
   STL-строка.
*/
class CharArray: private std::vector<char>
{
public:
  typedef std::vector<char> BaseType;
public:
  CharArray(): BaseType() { /* пусто */ }
  CharArray(const BaseType& v): BaseType(v) { /* пусто */ }

  ~CharArray() { /* пусто */ }

  CharArray(const CharArray& other): BaseType(other) { /* пусто */ }

  CharArray& operator=(const CharArray& other) {
    BaseType::operator=(other);
    return *this;
  }

  CharArray& operator=(const BaseType& other) {
    BaseType::operator=(other);
    return *this;
  }

  typedef BaseType& BaseTypeRef;
  typedef const BaseType& ConstBaseTypeRef;

  BaseTypeRef as_vector() {
    return *this;
  }

  ConstBaseTypeRef as_vector() const {
    return *this;
  }

  const char *c_str(unsigned offset = 0) const {
    return &at(offset);
  }

  char *c_str(unsigned offset = 0) {
    return &at(offset);
  }

  unsigned strlen(unsigned offset = 0) const {
    return ::strlen( &at(offset) );
  }

  void swap(CharArray& other) {
    BaseType::swap(other.as_vector());
  }

  using BaseType::push_back;
  using BaseType::back;
  using BaseType::pop_back;
  using BaseType::front;

  using BaseType::begin;
  using BaseType::end;
  using BaseType::const_iterator;
  using BaseType::iterator;

  using BaseType::size;
  using BaseType::resize;
  using BaseType::capacity;
  using BaseType::reserve;

  using BaseType::insert;
  using BaseType::erase;
  using BaseType::clear;

  using BaseType::at;
  using BaseType::operator[];
  using BaseType::empty;
  using BaseType::assign;
};

refalrts::FnResult string_from_seq(
  CharArray& string, refalrts::Iter begin, refalrts::Iter end
) {
  CharArray result;

  enum { cBufLen = 100 };
  char buffer[cBufLen + 1] = { 0 };

  for( ; ; ) {
    unsigned read = refalrts::read_chars(buffer, cBufLen, begin, end);

    if( read == 0 ) {
      break;
    }

    // На строчку ниже ругается BCC55, поэтому она закомментирована.
    // Компиляторы g++ и MS VC обрабатывают без ошибок.
    //result.insert( result.end(), buffer, buffer + read );

    CharArray::iterator end = result.end();

    result.as_vector().insert( end, buffer, buffer + read );
  }

  /*
    Здесь empty_seq( begin, end ) || (begin->tag != cDataChar).
  */

  if( refalrts::empty_seq( begin, end ) ) {
    result.push_back( '\0' );
    string.swap( result );
    return refalrts::cSuccess;
  } else {
    // здесь begin->tag != cDataChar
    return refalrts::cRecognitionImpossible;
  }
}

/*
  Массив для распределяемых в динамической памяти фрагментов.
  Позволяет последовательно накапливать указатели на фрагменты,
  дабы затем их в цикле сконкатенировать.
  Используется на третьей фазе обработки предложения
*/
class AllocArray
{
public:
  // Конструктор по умолчанию создаётся компилятором.
  // Невиртуальный деструктор создаётся компилятором.
  // Конструктор копирования создаётся компилятором.
  // Оператор присваивания создаётся компилятором.

  /*
    Операции push_back/push_front перепутаны по следующей причине:
    склеивание происходит в обратном направлении функцией
    splice_evar, просмотр контейнера происходит в прямом направлении.
  */

  // Добавление одноэлементного узла.
  void push_front(refalrts::Iter element) {
    m_fragments.push_back( std::make_pair(element, element) );
  }
  void push_back(refalrts::Iter element) {
    m_fragments.push_front( std::make_pair(element, element) );
  }

  // Добавление диапазона узлов.
  void push_front(refalrts::Iter begin, refalrts::Iter end) {
    m_fragments.push_back( std::make_pair(begin, end) );
  }
  void push_back(refalrts::Iter begin, refalrts::Iter end) {
    m_fragments.push_front( std::make_pair(begin, end) );
  }

  // Сборка результата.
  refalrts::Iter build(refalrts::Iter res);

private:
  typedef std::pair<refalrts::Iter, refalrts::Iter> IterPair;
  typedef std::deque<IterPair> Fragments;
  Fragments m_fragments;
};

refalrts::Iter AllocArray::build(refalrts::Iter res) {
  Fragments::iterator begin = m_fragments.begin();
  Fragments::iterator end = m_fragments.end();

  while( begin != end ) {
    res = refalrts::splice_evar( res, begin->first, begin->second );
    ++begin;
  }

  return res;
}

/*
  Константа используется для утверждений после метки default внутри switch.
  Использование: assert( SWITCH_VIOLATION );
*/
enum { SWITCH_VIOLATION = 0 };

} // unnamed namespace

/*===========================================================================
  Реализация функций библиотеки.
===========================================================================*/

// Реализация базовых математических функций:

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

  // Образец: <Open s.Mode e.FileName>

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

  const char ch_mode = sMode->char_info;
  const char *str_mode =
    (ch_mode == 'r') ? "rt" :
    (ch_mode == 'w') ? "wt" :
    (ch_mode == 'a') ? "at" : 0;

  if( str_mode == 0 ) return cRecognitionImpossible;

  CharArray file_name;

  FnResult name_parse = string_from_seq( file_name, arg_begin, arg_end );

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
        printf_res = fprintf(f, "%d ", ptr->number_info);
        continue;

      case cDataFunction:
        printf_res = fprintf(f, "%s ", (ptr->function_info.name)());
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

refalrts::FnResult implement_fileio::std_handle(
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
    char value = ptr->char_info;
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

REFAL_FUNC(implement_os::command_line) {
  // Формат <CommandLine> == e.CommandLine

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  // Формат аргумента --- пустое выражение

  if( ! empty_seq(arg_begin, arg_end) ) return cRecognitionImpossible;

  const char *cmd_line = GetCommandLine();
  unsigned len = strlen(cmd_line);
  Iter cmd_b = 0, cmd_e = 0;

  reset_allocator();

  if( ! alloc_chars(cmd_b, cmd_e, cmd_line, len) ) return cNoMemory;

  // Построение результата
  splice_evar(open_call, cmd_b, cmd_e);
  splice_to_freelist(open_call, close_call);
  return cSuccess;
}

REFAL_FUNC(implement_os::env_list) {
  // Формат <EnvList> == (e.EnvLine)*

  using namespace refalrts;

  // Скобки вызова, имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  // Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  // Аргумент --- пустое выражение

  if( ! empty_seq(arg_begin, arg_end) ) return cRecognitionImpossible;

  char *env_block = static_cast<char*>(GetEnvironmentStrings());

  // Возможно, не слишком хорошее решение.
  if( env_block == 0 ) return cRecognitionImpossible;

  // Распределение памяти
  AllocArray allocs;
  reset_allocator();

  for(
    char *cur_line = env_block;
    *cur_line != '\0';
    cur_line += strlen(cur_line) + 1
  ) {
    Iter open_bracket = 0, close_bracket = 0;
    Iter env_begin = 0, env_end = 0;

    bool allocated =
      alloc_open_bracket(open_bracket)
      && alloc_string(env_begin, env_end, cur_line)
      && alloc_close_bracket(close_bracket);

    if( ! allocated ) return cNoMemory;

    link_brackets(open_bracket, close_bracket);

    allocs.push_back(open_bracket);
    allocs.push_back(env_begin, env_end);
    allocs.push_back(close_bracket);
  }

  FreeEnvironmentStrings(env_block);

  // Построение результата
  allocs.build(open_call);
  splice_to_freelist(open_call, close_call);
  return cSuccess;
}

DECL_REFAL_IDENT(Success, "Success");
DECL_REFAL_IDENT(Wait, "Wait");
DECL_REFAL_IDENT(NoWait, "NoWait");

REFAL_FUNC(implement_os::create_process) {
  /*
    Функция создания нового процесса,
    Вызывает одноимённую WinAPI-функцию.

    Формат:
      <CreateProcess s.Wait (e.Program) (e.CommandLine)>
        == #Success s.RetCode
        == #Success
        == #Fails

      s.Wait ::= #Wait | #NoWait

    При выборе режима #Wait выполнение текущей программы приостанавливается
    до тех пор, пока не закончит выполняться указанная программа.
    При выборе режима #NoWait запускается указанная программа и сразу же
    после этого управление возвращается в "материнскую" программу ---
    обе программы продолжают выполняться параллельно.

    Параметр s.RetCode, представляющий собой код возврата программы,
    возвращается только в режиме #Wait.
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  // Сопоставление с образцом
  Iter sWait = 0;
  Iter eProgram_b = 0, eProgram_e = 0;
  Iter eCmdLine_b = 0, eCmdLine_e = 0;
  CharArray program, cmdline;
  bool matched =
    svar_left( sWait, arg_begin, arg_end )
    && ( cDataIdentifier == sWait->tag )
    && (
         (REFAL_IDENT(Wait) == sWait->ident_info)
         || (REFAL_IDENT(NoWait) == sWait->ident_info)
       )
    && brackets_left( eProgram_b, eProgram_e, arg_begin, arg_end )
    && (string_from_seq(program, eProgram_b, eProgram_e) == cSuccess)
    && brackets_left( eCmdLine_b, eCmdLine_e, arg_begin, arg_end )
    && (string_from_seq(cmdline, eCmdLine_b, eCmdLine_e) == cSuccess)
    && empty_seq( arg_begin, arg_end );

  if( matched == false ) return cRecognitionImpossible;

  bool wait_mode = (REFAL_IDENT(Wait) == sWait->ident_info);
  char *str_program = (program.strlen() ? program.c_str() : NULL);
  char *str_cmdline = (cmdline.strlen() ? cmdline.c_str() : NULL);

  // Вызов API-функции

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  bool process_started = CreateProcess(
    str_program, // Имя исполняемого модуля программы.
    str_cmdline, // Командная строка программы.
    NULL, // Дескрипторы процесса не наследуются.
    NULL, // Дескрипторы потока не наследуются.
    FALSE, // Установка наследования потоков в ЛОЖЬ.
    0, // Флаги создания не заданы.
    NULL, // Использовать родительские переменные среды.
    NULL, // Использовать родительскую текущую папку.
    &si,
    &pi
  );

  // Начиная с этого итератора фрагмент поля зрения выбрасывается
  Iter garbage_start = func_name;

  if( process_started ) {
    // Процесс запущен

    if( wait_mode ) {
      // Ожидаем завершения программы и получаем код возврата
      DWORD ret_code = 0;

      WaitForSingleObject( pi.hProcess, INFINITE );
      GetExitCodeProcess( pi.hProcess, & ret_code );

      /*
        Повторно используем имя функции, т.к. открывающая скобка активации
        оставлена для возврата #Success/#Fails.
      */

      func_name->function_info.ptr = 0;
      func_name->function_info.name = 0;
      func_name->tag = cDataNumber;
      func_name->number_info = ret_code;

      // Первые два узла используются --- выбрасывать начинаем с третьего.
      garbage_start = sWait;
    }

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    // Переинициализация открывающей скобки как #Success
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Success);
  } else {
    // Переинициализация открывающей скобки как #Fails
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Fails);
  }

  // Очистка поля зрения и завершение
  splice_to_freelist( garbage_start, close_call );
  return cSuccess;
}

namespace {

// Функция, порождающая атрибутную информацию (объявление)
bool add_attribute_record(
  AllocArray& allocs,
  const FILETIME& creation_time,
  const FILETIME& last_access_time,
  const FILETIME& last_write_time,
  DWORD file_attributes,
  DWORD size
);

} // безымянное namespace

REFAL_FUNC(implement_fs::find_files) {
  /*
    Функция является оболочкой над парой WinAPI-функций
    FindFirstFile и FindNextFile.

    Формат функции:
      <FindFiles e.Mask>
        == #Success ((e.FileName) e.Attribs)*
        == #Fails

      e.Attribs = (s.Attribute e.Value)*

    s.Attribute может принимать следующие значения:
      * #A-DateTime, #M-DateTime и #C-DateTime --- соответственно
        даты доступа, модификации и создания файла;
        формат e.Value:
          s.Year s.Month s.Day s.Hour s.Min s.Sec s.Millisec
      * #Size --- размер файла;
        формат e.Value:
          s.NumberSize | #Dir,
        соответственно размер файла в виде числа для регулярного файла
        или символ #Dir для папки.

    В дальнейшем возможно расширение набора атрибутов файла собственно
    собственно атрибутами: только для чтения, скрытый, архивный...
  */

  using namespace refalrts;

  // Скобки вызова, имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  // Сопоставление с образцом
  CharArray filemask;
  FnResult match = string_from_seq(filemask, arg_begin, arg_end);

  if( cSuccess != match ) return cRecognitionImpossible;

  // Построение списка каталогов в поле зрения

  WIN32_FIND_DATA find_file_data = { 0 };

  HANDLE hFind = FindFirstFile(filemask.c_str(), &find_file_data);

  if( hFind != INVALID_HANDLE_VALUE ) {
    // Открыли папку для просмотра, сканируем
    AllocArray allocs;

    reset_allocator();

    do {
      Iter open_main_bracket = 0, close_main_bracket = 0;
      Iter open_filename_bracket = 0, close_filename_bracket = 0;
      Iter filename_b = 0, filename_e = 0;

      bool allocated =
        alloc_open_bracket(open_main_bracket)
        && alloc_close_bracket(close_main_bracket)
        && alloc_open_bracket(open_filename_bracket)
        && alloc_close_bracket(close_filename_bracket)
        && alloc_string(filename_b, filename_e, find_file_data.cFileName);

      if( ! allocated ) return cRecognitionImpossible;

      link_brackets(open_main_bracket, close_main_bracket);
      link_brackets(open_filename_bracket, close_filename_bracket);

      allocs.push_back(open_main_bracket);
      allocs.push_back(open_filename_bracket);
      allocs.push_back(filename_b, filename_e);
      allocs.push_back(close_filename_bracket);

      allocated = add_attribute_record(
        allocs,
        find_file_data.ftCreationTime,
        find_file_data.ftLastAccessTime,
        find_file_data.ftLastWriteTime,
        find_file_data.dwFileAttributes,
        find_file_data.nFileSizeLow
      );

      if( ! allocated ) return cRecognitionImpossible;

      allocs.push_back(close_main_bracket);
        
    } while( FindNextFile(hFind, &find_file_data) != 0 );

    // Просмотр папки мог прерваться и ошибкой

    DWORD dwError = GetLastError();
    FindClose(hFind);

    if( ERROR_NO_MORE_FILES != dwError ) {
      // Переинициализируем open_call,
      // результат вставляем перед именем функции.
      open_call->link_info = 0;
      open_call->tag = cDataIdentifier;
      open_call->ident_info = REFAL_IDENT(Success);

      allocs.build( func_name );
      splice_to_freelist( func_name, close_call );
      return cSuccess;
    } else {
      // Чтение завершилось ошибкой
      // Переинициализация открывающей скобки
      open_call->link_info = 0;
      open_call->tag = cDataIdentifier;
      open_call->ident_info = REFAL_IDENT(Fails);

      splice_to_freelist( func_name, close_call );
      return cSuccess;
    }

  } else {
    // Открыть папку для просмотра не удалось
    // Переинициализируем открывающую угловую скобку
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Fails);
    
    splice_to_freelist(func_name, close_call);
    return cSuccess;
  }
}

DECL_REFAL_IDENT(AD_DateTime, "A-DateTime");
DECL_REFAL_IDENT(CD_DateTime, "C-DateTime");
DECL_REFAL_IDENT(MD_DateTime, "M-DateTime");
DECL_REFAL_IDENT(Size, "Size");
DECL_REFAL_IDENT(Dir, "Dir");

namespace {

bool append_filetime(
  AllocArray& allocs,
  refalrts::RefalIdentifier ident,
  const FILETIME& time
);

// Функция, порождающая атрибутную информацию (определение)
bool add_attribute_record(
  AllocArray& allocs,
  const FILETIME& creation_time,
  const FILETIME& last_access_time,
  const FILETIME& last_write_time,
  DWORD file_attributes,
  DWORD size
) {
  using namespace refalrts;

  bool append_time =
    append_filetime(allocs, REFAL_IDENT(CD_DateTime), creation_time)
    && append_filetime(allocs, REFAL_IDENT(AD_DateTime), last_access_time)
    && append_filetime(allocs, REFAL_IDENT(MD_DateTime), last_write_time);

  if( ! append_time ) return false;

  Iter open_bracket_size = 0, close_bracket_size = 0;
  Iter size_ident = 0, size_value = 0;

  bool is_directory = (file_attributes & FILE_ATTRIBUTE_DIRECTORY);
  bool size_allocs =
    alloc_open_bracket(open_bracket_size)
    && alloc_ident(size_ident, REFAL_IDENT(Size))
    &&
      (
        is_directory
        ? alloc_ident(size_value, REFAL_IDENT(Dir))
        : alloc_number(size_value, size)
      )
    && alloc_close_bracket(close_bracket_size);

  if( ! size_allocs ) return false;

  link_brackets(open_bracket_size, close_bracket_size);
  allocs.push_back(open_bracket_size);
  allocs.push_back(size_ident);
  allocs.push_back(size_value);
  allocs.push_back(close_bracket_size);

  return true;
}

bool append_filetime(
  AllocArray& allocs,
  refalrts::RefalIdentifier ident,
  const FILETIME& time
) {
  using namespace refalrts;

  SYSTEMTIME file_system_time = { 0 };
  FileTimeToSystemTime( & time, & file_system_time );

  Iter open_bracket = 0, close_bracket = 0;
  Iter time_ident = 0;
  Iter year = 0;
  Iter month = 0;
  Iter day = 0;
  Iter hour = 0;
  Iter minute = 0;
  Iter second = 0;
  Iter milliseconds = 0;

  bool alloc_time =
    alloc_open_bracket( open_bracket )
    && alloc_close_bracket( close_bracket )
    && alloc_ident( time_ident, ident )
    && alloc_number( year, file_system_time.wYear )
    && alloc_number( month, file_system_time.wMonth )
    && alloc_number( day, file_system_time.wDay )
    && alloc_number( hour, file_system_time.wHour )
    && alloc_number( minute, file_system_time.wMinute )
    && alloc_number( second, file_system_time.wSecond )
    && alloc_number( milliseconds, file_system_time.wMilliseconds );

  if( ! alloc_time ) return false;

  link_brackets( open_bracket, close_bracket );
  allocs.push_back( open_bracket );
  allocs.push_back( time_ident );
  allocs.push_back( year );
  allocs.push_back( month );
  allocs.push_back( day );
  allocs.push_back( hour );
  allocs.push_back( minute );
  allocs.push_back( second );
  allocs.push_back( milliseconds );
  allocs.push_back( close_bracket );

  return true;
}

} // безымянное namespace

REFAL_FUNC(implement_fs::file_attributes) {
  /*
    Функция получает имя файла и возвращает его атрибуты.

    Формат:
      <FileAttributes e.FileName>
        == #Success e.Attributes
        == #Fails

    e.Attributes имеет тот же формат, что и в функции FindFiles.
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left( arg_begin, arg_end );
  Iter func_name = arg_begin;
  move_left( arg_begin, arg_end );
  Iter close_call = arg_end;
  move_right( arg_begin, arg_end );

  CharArray file_name;
  FnResult match_name = string_from_seq(file_name, arg_begin, arg_end);

  if( cSuccess != match_name ) return cRecognitionImpossible;

  // Запрос атрибутов файла
  WIN32_FILE_ATTRIBUTE_DATA attributes = { 0 };
  BOOL get_attribute = GetFileAttributesEx(
    file_name.c_str(),
    GetFileExInfoStandard,
    & attributes
  );

  if( get_attribute ) {
    // Получение атрибутов удалось
    
    // Распределяем память для результата
    reset_allocator();

    AllocArray allocs;
    bool alloc_attribs = add_attribute_record(
      allocs,
      attributes.ftCreationTime,
      attributes.ftLastAccessTime,
      attributes.ftLastWriteTime,
      attributes.dwFileAttributes,
      attributes.nFileSizeLow
    );

    if( ! alloc_attribs ) return cNoMemory;

    // Переиспользуем открывающую угловую сбоку,
    // вставляем перед именем функции
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Success);

    allocs.build( func_name );
    splice_to_freelist( func_name, close_call );
    return cSuccess;
  } else {
    // Чтение атрибутов не удалось
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Fails);

    splice_to_freelist( func_name, close_call );
    return cSuccess;
  }
}

REFAL_FUNC(implement_fs::full_name) {
  /*
    Возвращает абсолютный путь для относительного вроде как.

    Формат:
      <FullName e.RelativePath>
        == Success e.FullPath
        == Fails

    Функция не гарантирует того, что сформированный путь существует.
  */

  using namespace refalrts;

  // Угловые скобки и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  CharArray relative_name;
  FnResult match_relative_name =
    string_from_seq(relative_name, arg_begin, arg_end);

  if( cSuccess != match_relative_name ) return match_relative_name;

  bool convert_ok;
  CharArray absolut_name;
  DWORD size = relative_name.strlen() + 1;

  for( ; ; ) {
    absolut_name.resize( size );
    char *p = 0;
    size = GetFullPathName(
      relative_name.c_str(),
      absolut_name.size(),
      absolut_name.c_str(),
      & p
    );

    /*
      Согласно документации на GetFullPathName, если имя файла успешно
      записано в предоставленный буфер, то функция возвращает длину имени
      исключая символ концевого нуля, т.е. size < absolut_name.size());
      если имя файла не влезает, то возвращается требуемая длина включая
      символ концевого нуля, т.е. size > absolut_name.size().
      Таким образом, возвращаемая длина никогда не будет равна размеру
      буфера.
    */
    if( (0 < size) && (size < absolut_name.size()) ) {
      // В размер буфера вписались, полный путь сформирован
      convert_ok = true;
      break;
    } else if( absolut_name.size() < size ) {
      // В размер буфера не вписались, увеличиваем его и повторяем
      continue;
    } else {
      assert( size == 0 );
      // Произошла ошибка формирования имени
      convert_ok = false;
      break;
    }
  }

  if( convert_ok ) {
    // Распределяем память для имени и помещаем его перед именем функции.
    reset_allocator();

    Iter eName_b = 0, eName_e = 0;
    if( ! alloc_string(eName_b, eName_e, absolut_name.c_str()) )
      return cNoMemory;

    splice_evar( func_name, eName_b, eName_e );
  }

  // Переиспользуем открывающую угловую скобку
  open_call->link_info = 0;
  open_call->tag = cDataIdentifier;
  open_call->ident_info =
    (convert_ok ? REFAL_IDENT(Success) : REFAL_IDENT(Fails));

  // Очиста поля зрения
  splice_to_freelist( func_name, close_call );
  return cSuccess;
}

refalrts::FnResult implement_fs::perform_file_operation(
  implement_fs::FileFunc operation,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  /*
    Функция выполняет заданную операцию над заданным файлом.
    Функция является промежуточной для функций создания каталога,
    удаления файла и удаления каталога.

    Формат:
      <ФайловаяФункция e.FileName> == #Success | #Fails
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left( arg_begin, arg_end );
  Iter func_name = arg_begin;
  move_left( arg_begin, arg_end );
  Iter close_call = arg_end;
  move_right( arg_begin, arg_end );

  CharArray filename;
  FnResult match_filename = string_from_seq(filename, arg_begin, arg_end);

  if( cSuccess != match_filename ) return match_filename;

  bool result = operation(filename.c_str());

  // Переинициализируем открывающую угловую скобку
  open_call->link_info = 0;
  open_call->tag = cDataIdentifier;
  open_call->ident_info =
    (result ? REFAL_IDENT(Success) : REFAL_IDENT(Fails));

  splice_to_freelist(func_name, close_call);
  return cSuccess;
}

bool implement_fs::make_dir(const char *filename) {
  return CreateDirectory(filename, 0);
}

bool implement_fs::remove_dir(const char *filename) {
  return RemoveDirectory(filename);
}

bool implement_fs::remove_file(const char *filename) {
  return DeleteFile(filename);
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
  FILE *flog,
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

  // Вывод дампа:
  if( 0 == flog ) {
    fprintf(stderr, "<Log>: ");
    flog = stderr;
  }

  refalrts::debug_print_expr(flog, arg_begin, arg_end);
  fprintf(flog, "\n");
  fflush(flog);

  // Очистка поля зрения
  if( transparent ) {
    splice_to_freelist( open_call, func_name );
    splice_to_freelist( close_call, close_call );
  } else {
    splice_to_freelist( open_call, close_call );
  }

  return cSuccess;
}

DECL_REFAL_IDENT(Exit, "@Exit");

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
  func_name->function_info.name = REFAL_IDENT(Exit);

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

FILE *implement_selfdiag::get_log_handle() {
  if( 0 == g_log_file ) {
    g_log_file = fopen("__log", "wt");
  }

  return g_log_file;
}



//#error No compile


