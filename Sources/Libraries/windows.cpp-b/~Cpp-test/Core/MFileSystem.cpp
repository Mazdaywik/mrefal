#include <windows.h>
#include <vector>
#include <string>
#include <utility>

#include "refalrts.h"


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

refalrts::FnResult CoreP_MFileSystemP_InitE_(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::this_is_generated_function();
  do {
    refalrts::Iter bb_0 = arg_begin;
    refalrts::Iter be_0 = arg_end;
    refalrts::move_left( bb_0, be_0 );
    refalrts::move_left( bb_0, be_0 );
    refalrts::move_right( bb_0, be_0 );
    // # Init
    if( ! refalrts::ident_left(  & InitL_<int>::name, bb_0, be_0 ) ) 
      break;
    if( ! empty_seq( bb_0, be_0 ) )
      break;

    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  do {
    refalrts::Iter bb_0 = arg_begin;
    refalrts::Iter be_0 = arg_end;
    refalrts::move_left( bb_0, be_0 );
    refalrts::move_left( bb_0, be_0 );
    refalrts::move_right( bb_0, be_0 );
    // # Final
    if( ! refalrts::ident_left(  & FinalL_<int>::name, bb_0, be_0 ) ) 
      break;
    if( ! empty_seq( bb_0, be_0 ) )
      break;

    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } while ( 0 );

  return refalrts::cRecognitionImpossible;
}

//$LABEL DateTime
template <typename T>
struct DateTimeL_ {
  static const char *name() {
    return "DateTime";
  }
};

//$LABEL Dir
template <typename T>
struct DirL_ {
  static const char *name() {
    return "Dir";
  }
};

//$LABEL LongName
template <typename T>
struct LongNameL_ {
  static const char *name() {
    return "LongName";
  }
};

//$LABEL Size
template <typename T>
struct SizeL_ {
  static const char *name() {
    return "Size";
  }
};

//$LABEL Fails
template <typename T>
struct FailsL_ {
  static const char *name() {
    return "Fails";
  }
};

//$LABEL Success
template <typename T>
struct SuccessL_ {
  static const char *name() {
    return "Success";
  }
};

namespace {

refalrts::FnResult string_from_seq(
  std::vector<char>& string, refalrts::Iter begin, refalrts::Iter end
) {
  std::vector<char> result;

  enum { cBufLen = 100 };
  char buffer[cBufLen + 1] = { 0 };

  for( ; ; ) {
    unsigned read = refalrts::read_chars(buffer, cBufLen, begin, end);

    if( read == 0 ) {
      break;
    }

    result.insert( result.end(), buffer, buffer + read );
  }

  /*
    Здесь empty_seq( begin, end ) || (begin->tag != cDataChar).
  */

  if( empty_seq( begin, end ) ) {
    result.push_back( '\0' );
    string.swap( result );
    return refalrts::cSuccess;
  } else {
    // здесь begin->tag != cDataChar
    return refalrts::cRecognitionImpossible;
  }
}

bool get_full_name( std::string& path ) {
  std::vector<char> vpath( path.begin(), path.end() );
  vpath.push_back( '\0' );

  std::vector<char> vresult( 100 );
  char *namepos = 0;

  DWORD pathlen;
  for( ; ; ) {
    pathlen = GetFullPathName(
      & vpath[0],
      vresult.size(),
      & vresult[0],
      & namepos
    );

    if( pathlen == 0 ) {
      return false;
    } else if( pathlen < vresult.size() ) {
      vresult.push_back( '\0' );
      path = & vresult[0];
      return true;
    } else {
      vresult.resize( pathlen + 1 );
      continue;
    }
  }
}

} // unnamed namespace

refalrts::FnResult CoreP_MFileSystemP_GetFileAttributes(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::Iter bb_0 = arg_begin;
  refalrts::Iter be_0 = arg_end;
  refalrts::move_left( bb_0, be_0 );
  refalrts::move_left( bb_0, be_0 );
  refalrts::move_right( bb_0, be_0 );
  refalrts::Iter eFileName_b_1;
  refalrts::Iter eFileName_e_1;
  // e.FileName
  eFileName_b_1 = bb_0;
  refalrts::use( eFileName_b_1 );
  eFileName_e_1 = be_0;
  refalrts::use( eFileName_e_1 );

  std::vector<char> filename;

  refalrts::FnResult res =
    string_from_seq(filename, eFileName_b_1, eFileName_e_1);

  if( res == refalrts::cRecognitionImpossible ) {
    return refalrts::cRecognitionImpossible;
  }

  WIN32_FIND_DATA find_file_data;

  HANDLE hFind = FindFirstFile( & filename[0], & find_file_data );

  if( hFind == INVALID_HANDLE_VALUE ) {
    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;

    refalrts::Iter nSF = 0;
    if( ! refalrts::alloc_ident( nSF, & FailsL_<int>::name ) )
      return refalrts::cNoMemory;

    res = refalrts::splice_elem( res, nSF );
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );

    return refalrts::cSuccess;
  } else {
    std::string full_name( find_file_data.cFileName );

    if( ! get_full_name( full_name ) ) {
      refalrts::reset_allocator();
      refalrts::Iter res = arg_begin;
    
      refalrts::Iter nSF = 0;
      if( ! refalrts::alloc_ident( nSF, & FailsL_<int>::name ) )
        return refalrts::cNoMemory;
    
      res = refalrts::splice_elem( res, nSF );
      refalrts::use( res );
      refalrts::splice_to_freelist( arg_begin, arg_end );
    
      return refalrts::cSuccess;
    } else {
      bool is_directory =
        ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

      SYSTEMTIME file_system_time = { 0 };

      FileTimeToSystemTime(
        & find_file_data.ftLastWriteTime, & file_system_time
      );

      refalrts::reset_allocator();
      refalrts::Iter res = arg_begin;
      refalrts::Iter nSF = 0;
      if( ! refalrts::alloc_ident( nSF, & SuccessL_<int>::name ) )
        return refalrts::cNoMemory;
      refalrts::Iter n0 = 0;
      if( ! refalrts::alloc_open_bracket( n0 ) )
        return refalrts::cNoMemory;
      refalrts::Iter n1 = 0;
      if( ! refalrts::alloc_ident( n1, & DateTimeL_<int>::name ) )
        return refalrts::cNoMemory;
      refalrts::Iter n2 = 0;
      if( ! refalrts::alloc_number( n2, file_system_time.wYear ) )
        return refalrts::cNoMemory;
      refalrts::Iter n3 = 0;
      if( ! refalrts::alloc_number( n3, file_system_time.wMonth ) )
        return refalrts::cNoMemory;
      refalrts::Iter n4 = 0;
      if( ! refalrts::alloc_number( n4, file_system_time.wDay ) )
        return refalrts::cNoMemory;
      refalrts::Iter n5 = 0;
      if( ! refalrts::alloc_number( n5, file_system_time.wHour ) )
        return refalrts::cNoMemory;
      refalrts::Iter n6 = 0;
      if( ! refalrts::alloc_number( n6, file_system_time.wMinute ) )
        return refalrts::cNoMemory;
      refalrts::Iter n7 = 0;
      if( ! refalrts::alloc_number( n7, file_system_time.wSecond ) )
        return refalrts::cNoMemory;
      refalrts::Iter n8 = 0;
      if( ! refalrts::alloc_number( n8, file_system_time.wMilliseconds ) )
        return refalrts::cNoMemory;
      refalrts::Iter n9 = 0;
      if( ! refalrts::alloc_close_bracket( n9 ) )
        return refalrts::cNoMemory;
      refalrts::Iter n10 = 0;
      if( ! refalrts::alloc_open_bracket( n10 ) )
        return refalrts::cNoMemory;
      refalrts::Iter n11 = 0;
      if( ! refalrts::alloc_ident( n11, & SizeL_<int>::name ) )
        return refalrts::cNoMemory;
      refalrts::Iter n12 = 0;

      if( is_directory ) {
        if( ! refalrts::alloc_ident( n12, & DirL_<int>::name ) )
          return refalrts::cNoMemory;
      } else {
        if( ! refalrts::alloc_number( n12, 1000UL ) )
          return refalrts::cNoMemory;
      }

      refalrts::Iter n14 = 0;
      if( ! refalrts::alloc_close_bracket( n14 ) )
        return refalrts::cNoMemory;
      refalrts::Iter n15 = 0;
      if( ! refalrts::alloc_open_bracket( n15 ) )
        return refalrts::cNoMemory;
      refalrts::Iter n16 = 0;
      if( ! refalrts::alloc_ident( n16, & LongNameL_<int>::name ) )
        return refalrts::cNoMemory;

      refalrts::Iter fullname_b = 0;
      refalrts::Iter fullname_e = 0;

      bool name_allocated =
        refalrts::alloc_string( fullname_b, fullname_e, full_name.c_str() );

      if( ! name_allocated )
        return refalrts::cNoMemory;

      refalrts::Iter n26 = 0;
      if( ! refalrts::alloc_close_bracket( n26 ) )
        return refalrts::cNoMemory;

      refalrts::link_brackets( n15, n26 );
      res = refalrts::splice_elem( res, n26 );
      res = refalrts::splice_evar( res, fullname_b, fullname_e );
      res = refalrts::splice_elem( res, n16 );
      res = refalrts::splice_elem( res, n15 );
      refalrts::link_brackets( n10, n14 );
      res = refalrts::splice_elem( res, n14 );
      res = refalrts::splice_elem( res, n12 );
      res = refalrts::splice_elem( res, n11 );
      res = refalrts::splice_elem( res, n10 );
      refalrts::link_brackets( n0, n9 );
      res = refalrts::splice_elem( res, n9 );
      res = refalrts::splice_elem( res, n8 );
      res = refalrts::splice_elem( res, n7 );
      res = refalrts::splice_elem( res, n6 );
      res = refalrts::splice_elem( res, n5 );
      res = refalrts::splice_elem( res, n4 );
      res = refalrts::splice_elem( res, n3 );
      res = refalrts::splice_elem( res, n2 );
      res = refalrts::splice_elem( res, n1 );
      res = refalrts::splice_elem( res, n0 );
      res = refalrts::splice_elem( res, nSF );
      refalrts::use( res );
      refalrts::splice_to_freelist( arg_begin, arg_end );
  
      return refalrts::cSuccess;
    }
  }
}

refalrts::FnResult CoreP_MFileSystemP_DirList(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::Iter bb_0 = arg_begin;
  refalrts::Iter be_0 = arg_end;
  refalrts::move_left( bb_0, be_0 );
  refalrts::move_left( bb_0, be_0 );
  refalrts::move_right( bb_0, be_0 );
  refalrts::Iter eDirectory_b_1;
  refalrts::Iter eDirectory_e_1;
  // e.Directory
  eDirectory_b_1 = bb_0;
  refalrts::use( eDirectory_b_1 );
  eDirectory_e_1 = be_0;
  refalrts::use( eDirectory_e_1 );

  std::vector<char> directory;

  refalrts::FnResult res =
    string_from_seq(directory, eDirectory_b_1, eDirectory_e_1);

  if( res == refalrts::cRecognitionImpossible ) {
    return refalrts::cRecognitionImpossible;
  }

  directory.back() = '\\'; // Перезаписываем нулевой символ в конце

  std::vector<char> find_str = directory;

  find_str.push_back('*');
  find_str.push_back('\0');

  WIN32_FIND_DATA find_file_data;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  hFind = FindFirstFile( & find_str[0], & find_file_data );

  if( hFind == INVALID_HANDLE_VALUE ) {
    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::Iter nSF = 0;
    if( ! refalrts::alloc_ident( nSF, & FailsL_<int>::name ) )
      return refalrts::cNoMemory;

    res = refalrts::splice_elem( res, nSF );
    refalrts::use( res );
    refalrts::splice_to_freelist( arg_begin, arg_end );
    return refalrts::cSuccess;
  } else {
    refalrts::reset_allocator();
    refalrts::Iter res = arg_begin;
    refalrts::Iter nSF = 0;
    if( ! refalrts::alloc_ident( nSF, & SuccessL_<int>::name ) )
      return refalrts::cNoMemory;

    std::vector< std::pair<refalrts::Iter, refalrts::Iter> > brackets;

    do {
      std::string name( find_file_data.cFileName );

      refalrts::Iter left_bracket;
      refalrts::Iter right_bracket;
      refalrts::Iter filename_b;
      refalrts::Iter filename_e;

      if( ! refalrts::alloc_open_bracket( left_bracket ) )
        return refalrts::cNoMemory;

      if( ! refalrts::alloc_string( filename_b, filename_e, name.c_str() ) )
        return refalrts::cNoMemory;

      if( ! refalrts::alloc_close_bracket( right_bracket ) )
        return refalrts::cNoMemory;

      refalrts::link_brackets( left_bracket, right_bracket );

      brackets.push_back( std::make_pair( left_bracket, right_bracket ) );
    } while( FindNextFile( hFind, & find_file_data ) != 0 );

    for( unsigned i = 0; i < brackets.size(); ++i ) {
      res = refalrts::splice_evar( res, brackets[i].first, brackets[i].second );
    }
    res = refalrts::splice_elem( res, nSF );

    refalrts::splice_to_freelist( arg_begin, arg_end );

    return refalrts::cSuccess;
  }
}

namespace {

extern "C" {

typedef BOOL (WINAPI *FileOperation)(LPCSTR filename);

}

refalrts::FnResult perform_file_operation(
  FileOperation operation,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  refalrts::Iter eName_b = arg_begin;
  refalrts::Iter eName_e = arg_end;

  refalrts::move_left( eName_b, eName_e );
  refalrts::move_left( eName_b, eName_e );
  refalrts::move_right( eName_b, eName_e );

  std::vector<char> filename;

  refalrts::FnResult res =
    string_from_seq( filename, eName_b, eName_e );

  if( res != refalrts::cSuccess ) {
    return res;
  }

  BOOL op_result = (*operation)( & filename[0] );

  refalrts::reset_allocator();

  refalrts::Iter nSF = 0;

  refalrts::RefalIdentifier ident =
    (op_result ? & SuccessL_<int>::name : & FailsL_<int>::name );

  if( ! refalrts::alloc_ident( nSF, ident ) )
    return refalrts::cNoMemory;

  refalrts::splice_elem( arg_begin, nSF );

  refalrts::splice_to_freelist( arg_begin, arg_end );

  return refalrts::cSuccess;
}

} // безымянное namespace

refalrts::FnResult CoreP_MFileSystemP_RemoveDir(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return perform_file_operation( & RemoveDirectory, arg_begin, arg_end );
}

refalrts::FnResult CoreP_MFileSystemP_RemoveFile(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return perform_file_operation( DeleteFile, arg_begin, arg_end );
}

namespace {

extern "C" BOOL WINAPI my_create_directory(LPCTSTR lpPathName) {
  return CreateDirectory(lpPathName, NULL);
}

} // безымянное namespace

refalrts::FnResult CoreP_MFileSystemP_MakeDir(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return perform_file_operation( my_create_directory, arg_begin, arg_end );
}

