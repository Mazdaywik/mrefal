#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <windows.h>

enum {
  cVER_MAJOR = 0,
  cVER_MINOR = 1
};

extern int dirlisting( const char *directory );
extern int fileinfo( const char *filename);

int main(int argc, char *argv[]) {
  if( (argc == 2) && (strcmp(argv[1], "VERSION") == 0) ) {
    printf("mr-extent#%d #%d", cVER_MAJOR, cVER_MINOR);
  } else if( (argc == 3) && (strcmp(argv[1], "LIST") == 0) ) {
    return dirlisting( argv[2] );
  } else if( (argc == 3) && (strcmp(argv[1], "INFO") == 0) ) {
    return fileinfo( argv[2] );
  } else {
    fprintf(stderr, "MR-EXTENT: Command line error\n");
    printf("E");
    return 1;
  }
  return 0;
}

void xxio_escape( std::string& text );
bool get_full_name( std::string& text );

int dirlisting( const char *directory ) {
  WIN32_FIND_DATA find_file_data;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string path(directory);
  path += "\\";

  hFind = FindFirstFile( (path + "*").c_str(), & find_file_data );
  if( hFind == INVALID_HANDLE_VALUE ) {
    printf("SF");
    return 1;
  } else {
    printf("SS");
    do {
      std::string name( /* path + */ find_file_data.cFileName );
      if( true || get_full_name( name ) ) {
        xxio_escape( name );
        printf( "(%s)\n", name.c_str() );
      } else {
        printf( "F" );
      }
    } while (FindNextFile( hFind, & find_file_data ) != 0);
    FindClose( hFind );
    return 0;
  }
}

int fileinfo( const char *filename ) {
  WIN32_FIND_DATA find_file_data;
  HANDLE hFind = FindFirstFile( filename, & find_file_data );
  if( hFind == INVALID_HANDLE_VALUE ) {
    printf( "SN" );
  } else {
    std::string full_name( find_file_data.cFileName );
    if( get_full_name(full_name) ) {
      if( (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ) {
        printf( "SD" );
      } else {
        printf( "S#%d ", find_file_data.nFileSizeLow );
      }

      SYSTEMTIME file_system_time = { 0 };
      FileTimeToSystemTime(
        & find_file_data.ftLastWriteTime, & file_system_time
      );

      xxio_escape( full_name );

      printf(
        "#%d #%d #%d #%d #%d #%d %s",
        file_system_time.wYear,
        file_system_time.wMonth,
        file_system_time.wDay,
        file_system_time.wHour,
        file_system_time.wMinute,
        file_system_time.wSecond,
        full_name.c_str()
      );
      FindClose( hFind );
    } else {
      printf("SF");
    }
  }
  return 0;
}

void xxio_escape( std::string& text ) {
  std::string result;
  for( unsigned i = 0; i < text.length(); ++i ) {
    char next = text[i];
    if( next == '\\' ) {
      result += "\\\\";
    } else if( next == '#' ) {
      result += "##";
    } else if( next == '(' ) {
      result += "#(";
    } else if( next == ')' ) {
      result += "#)";
    } else {
      result += next;
    }
  }

  text = result;
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

