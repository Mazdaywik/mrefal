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

refalrts::FnResult CoreBEP_MSelfDiagP_InitE_(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
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

refalrts::FnResult CoreBEP_MSelfDiagP_CloseLog(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::close_log(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MSelfDiagP_ExitFailure(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::exit_failure(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MSelfDiagP_LogD_T(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::log(
    implement_selfdiag::get_log_handle(),
    true, // прозрачный режим
    arg_begin, arg_end
  );
}

refalrts::FnResult CoreBEP_MSelfDiagP_Log(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::log(
    implement_selfdiag::get_log_handle(),
    false, // непрозрачный режим
    arg_begin, arg_end
  );
}

refalrts::FnResult CoreBEP_MSelfDiagP_PrintMessage(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_selfdiag::log(
    implement_selfdiag::get_stderror_handle(),
    false, // непрозрачный режим
    arg_begin, arg_end
  );
}

