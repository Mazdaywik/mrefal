#include "refalrts.h"
#include "mr_common.h"

#ifndef InitDEFINED_R_
#define InitDEFINED_R_

//$LABEL Init
template <typename T>
struct Init {
  static const char *name() {
    return "Init";
  }
};

#endif // InitDEFINED_R_

#ifndef FinalDEFINED_R_
#define FinalDEFINED_R_

//$LABEL Final
template <typename T>
struct Final {
  static const char *name() {
    return "Final";
  }
};

#endif // FinalDEFINED_R_

refalrts::FnResult CoreBEP_MStringsP_InitE_(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::this_is_generated_function();
  static unsigned s_counter = 0;
  refalrts::use_counter(s_counter);
  do {
    refalrts::Iter bb_0_0 = arg_begin;
    refalrts::Iter be_0_0 = arg_end;
    refalrts::move_left( bb_0_0, be_0_0 );
    refalrts::move_left( bb_0_0, be_0_0 );
    refalrts::move_right( bb_0_0, be_0_0 );
    // # Init
    //(0 0 )
    if( ! refalrts::ident_left(  & Init<int>::name, bb_0_0, be_0_0 ) ) 
      break;
    if( ! refalrts::empty_seq( bb_0_0, be_0_0 ) )
      break;

    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  do {
    refalrts::Iter bb_0_0 = arg_begin;
    refalrts::Iter be_0_0 = arg_end;
    refalrts::move_left( bb_0_0, be_0_0 );
    refalrts::move_left( bb_0_0, be_0_0 );
    refalrts::move_right( bb_0_0, be_0_0 );
    // # Final
    //(0 0 )
    if( ! refalrts::ident_left(  & Final<int>::name, bb_0_0, be_0_0 ) ) 
      break;
    if( ! refalrts::empty_seq( bb_0_0, be_0_0 ) )
      break;

    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  return refalrts::cRecognitionImpossible;
}

refalrts::FnResult CoreBEP_MStringsP_Chr(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::convert(
    implement_strings::chr, arg_begin, arg_end
  );
}

refalrts::FnResult CoreBEP_MStringsP_Ord(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::convert(
    implement_strings::ord, arg_begin, arg_end
  );
}

refalrts::FnResult CoreBEP_MStringsP_Symb(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::symb(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MStringsP_Numb(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::numb(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MStringsP_SerializeAtom(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_strings::serialize_atom(arg_begin, arg_end);
}

