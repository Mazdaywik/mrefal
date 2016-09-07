#include "refalrts.h"
#include "mr_common.h"
#include "mr_oslib.h"

#ifndef InitDEFINED_R_
#define InitDEFINED_R_
DECL_REFAL_IDENT(Init, "Init");
#endif // InitDEFINED_R_

#ifndef FinalDEFINED_R_
#define FinalDEFINED_R_
DECL_REFAL_IDENT(Final, "Final");
#endif // FinalDEFINED_R_

REFAL_FUNC(CoreBEP_OSP_MOSP_InitE_, "CoreBE.OS.MOS.Init#ENTRY") {
  refalrts::this_is_generated_function();
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

REFAL_FUNC(CoreBEP_OSP_MOSP_CreateProcess, "CoreBE.OS.MOS.CreateProcess") {
  return implement_os::create_process(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_OSP_MOSP_ForseExit, "CoreBE.OS.MOS.ForseExit") {
#ifdef MODULE_REFAL_NEW
  return ExitE_.ptr(arg_begin, arg_end);
#else
  return ExitE_(arg_begin, arg_end);
#endif
}

REFAL_FUNC(CoreBEP_OSP_MOSP_EnvList, "CoreBE.OS.MOS.EnvList") {
  return implement_os::env_list(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_OSP_MOSP_CommandLine, "CoreBE.OS.MOS.CommandLine") {
  return implement_os::command_line(arg_begin, arg_end);
}

