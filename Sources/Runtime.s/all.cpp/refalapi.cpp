#include "refalrts.h"
#include "refalapi.h"

refalrts::FnResult refalapi::CharArray::from_sequence(
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

    // В текущей версии Open Watcom (и форка Open Watcom V2) есть ошибка
    // в функции vector::insert, которая возникает в случае,
    // если во время вставки вектор увеличивает свою ёмкость (capacity).
    // Для обхода этой ошибки предварительно резервируем место.
    result.reserve( result.size() + read + 1 );

    // Без .as_vector() компилятор BCC 5.5 не может откомпилировать
    result.as_vector().insert( result.end(), buffer, buffer + read );
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

refalrts::Iter refalapi::AllocArray::build(refalrts::Iter res) {
  Fragments::iterator begin = m_fragments.begin();
  Fragments::iterator end = m_fragments.end();

  while( begin != end ) {
    res = refalrts::splice_evar( res, begin->first, begin->second );
    ++begin;
  }

  return res;
}
