#include "refalrts.h"
#include "mr_common.h"

#ifndef InitDEFINED_R_
#define InitDEFINED_R_
DECL_REFAL_IDENT(Init, "Init");
#endif // InitDEFINED_R_

#ifndef FinalDEFINED_R_
#define FinalDEFINED_R_
DECL_REFAL_IDENT(Final, "Final");
#endif // FinalDEFINED_R_

REFAL_FUNC(CoreBEP_MStringsP_InitE_, "CoreBE.MStrings.Init#ENTRY") {
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
    if( ! refalrts::ident_left( REFAL_IDENT(Init), bb_0_0, be_0_0 ) )
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
    if( ! refalrts::ident_left( REFAL_IDENT(Final), bb_0_0, be_0_0 ) )
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

REFAL_FUNC(CoreBEP_MStringsP_Chr, "CoreBE.MStrings.Chr") {
  return implement_strings::convert(
    implement_strings::chr, arg_begin, arg_end
  );
}

REFAL_FUNC(CoreBEP_MStringsP_Ord, "CoreBE.MStrings.Ord") {
  return implement_strings::convert(
    implement_strings::ord, arg_begin, arg_end
  );
}

REFAL_FUNC(CoreBEP_MStringsP_Symb, "CoreBE.MStrings.Symb") {
  return implement_strings::symb(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MStringsP_Numb, "CoreBE.MStrings.Numb") {
  return implement_strings::numb(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MStringsP_SerializeAtom, "CoreBE.MStrings.SerializeAtom") {
  return implement_strings::serialize_atom(arg_begin, arg_end);
}

