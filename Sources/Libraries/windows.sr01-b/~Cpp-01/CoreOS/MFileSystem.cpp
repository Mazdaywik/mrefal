// Automatically generated file. Don't edit!
#include "refalrts.h"
#include "baselib.h"


//$LABEL Init
template <typename T>
struct InitL_ {
  static const char *name() {
    return "Init";
  }
};

//$LABEL Final
template <typename T>
struct FinalL_ {
  static const char *name() {
    return "Final";
  }
};

refalrts::FnResult CoreOSP_MFileSystemP_InitE_(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
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
    if( ! refalrts::ident_left(  & InitL_<int>::name, bb_0_0, be_0_0 ) ) 
      break;
    if( ! empty_seq( bb_0_0, be_0_0 ) )
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
    if( ! refalrts::ident_left(  & FinalL_<int>::name, bb_0_0, be_0_0 ) ) 
      break;
    if( ! empty_seq( bb_0_0, be_0_0 ) )
      break;

    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  return refalrts::cRecognitionImpossible;
}

refalrts::FnResult CoreOSP_MFileSystemP_RemoveFile(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::perform_file_operation(
    implement_fs::remove_file, arg_begin, arg_end
  );
}

refalrts::FnResult CoreOSP_MFileSystemP_RemoveDir(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::perform_file_operation(
    implement_fs::remove_dir, arg_begin, arg_end
  );
}

refalrts::FnResult CoreOSP_MFileSystemP_MakeDir(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::perform_file_operation(
    implement_fs::make_dir, arg_begin, arg_end
  );
}

refalrts::FnResult CoreOSP_MFileSystemP_FullName(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::full_name(arg_begin, arg_end);
}

refalrts::FnResult CoreOSP_MFileSystemP_FileAttributes(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::file_attributes(arg_begin, arg_end);
}

refalrts::FnResult CoreOSP_MFileSystemP_FindFiles(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_fs::find_files(arg_begin, arg_end);
}

