#ifndef RefalAPI_H_
#define RefalAPI_H_

#include <deque>
#include <vector>
#include <string.h>
#include <utility>

#include "refalrts.h"
#include "refalmacro.h"

/*===========================================================================
  Утилитарные функции для работы с промежуточным представлением поля зрения.
===========================================================================*/

namespace refalapi {

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

  /*
    Если поддиапазон [arg_begin, arg_end] содержит только
    символы (characters), то эта функция возвращает
    эту последовательность символов как строку CharArray& string
    (при этом если string до вызова функции содержала какое-то
    значение, оно стирается) и возвращает refalrts::cSuccess.

    Строка string в этом случае всегда дополняется концевым нулём.

    Если же поддиапазон не состоит только из символов, то функция
    возвращает refalrts::cRecognitionImpossible, параметр string
    не изменяет.
  */
  static refalrts::FnResult from_sequence(
    CharArray& string, refalrts::Iter arg_begin, refalrts::Iter arg_end
  );

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

/*
  Константа используется для утверждений после метки default внутри switch.
  Использование: assert( SWITCH_VIOLATION );
*/
enum { SWITCH_VIOLATION = 0 };

} // namespace refalapi

using refalapi::SWITCH_VIOLATION;

#endif //RefalAPI_H_
