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

REFAL_FUNC(CoreBEP_MFileIOP_InitE_, "CoreBE.MFileIO.Init#ENTRY") {
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
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  return refalrts::cRecognitionImpossible;
}

REFAL_FUNC(CoreBEP_MFileIOP_StdOut, "CoreBE.MFileIO.StdOut") {
  return implement_fileio::get_stdout(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MFileIOP_StdIn, "CoreBE.MFileIO.StdIn") {
  return implement_fileio::get_stdin(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MFileIOP_ReadLine, "CoreBE.MFileIO.ReadLine") {
  return implement_fileio::read_line(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MFileIOP_Write, "CoreBE.MFileIO.Write") {
  return implement_fileio::write(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MFileIOP_Close, "CoreBE.MFileIO.Close") {
  return implement_fileio::close(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MFileIOP_Open, "CoreBE.MFileIO.Open") {
  return implement_fileio::open(arg_begin, arg_end);
}

REFAL_FUNC(CoreBEP_MFileIOP_FlushIO, "CoreBE.MFileIO.FlushIO") {
  refalrts::splice_to_freelist(arg_begin, arg_end);
  return refalrts::cSuccess;
}
