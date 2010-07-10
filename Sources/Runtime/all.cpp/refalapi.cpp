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

