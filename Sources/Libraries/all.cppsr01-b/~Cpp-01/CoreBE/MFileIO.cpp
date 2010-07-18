#include "refalrts.h"
#include "mr_common.h"


//$LABEL Init
template <typename T>
struct Init {
  static const char *name() {
    return "Init";
  }
};

//$LABEL Final
template <typename T>
struct Final {
  static const char *name() {
    return "Final";
  }
};

refalrts::FnResult CoreBEP_MFileIOP_InitE_(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::this_is_generated_function();
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
    if( ! empty_seq( bb_0_0, be_0_0 ) )
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
    if( ! refalrts::ident_left(  & Final<int>::name, bb_0_0, be_0_0 ) ) 
      break;
    if( ! empty_seq( bb_0_0, be_0_0 ) )
      break;

    refalrts::reset_allocator();
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  return refalrts::cRecognitionImpossible;
}

refalrts::FnResult CoreBEP_MFileIOP_StdOut(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::std_handle(stdout, arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MFileIOP_StdIn(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::std_handle(stdin, arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MFileIOP_ReadLine(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::read_line(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MFileIOP_Write(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::write(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MFileIOP_Close(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::close(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MFileIOP_Open(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fileio::open(arg_begin, arg_end);
}

refalrts::FnResult CoreBEP_MFileIOP_FlushIO(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  refalrts::splice_to_freelist(arg_begin, arg_end);
  return refalrts::cSuccess;
}
