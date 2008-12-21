#include <stdio.h>
#include <string.h>
#include <string>
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
    return 1;
  }
  return 0;
}

int dirlisting( const char *directory ) {
  WIN32_FIND_DATA find_file_data;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string path(directory);
  path += "\\*";

  hFind = FindFirstFile( path.c_str(), & find_file_data );
  if( hFind == INVALID_HANDLE_VALUE ) {
    printf("F");
    return 1;
  } else {
    printf("S");
    do {
      printf( "(%s)\n", find_file_data.cFileName );
    } while (FindNextFile( hFind, & find_file_data ) != 0);
    FindClose( hFind );
    return 0;
  }
}

int fileinfo( const char *filename ) {
  WIN32_FIND_DATA find_file_data;
  HANDLE hFind = FindFirstFile( filename, & find_file_data );
  if( hFind == INVALID_HANDLE_VALUE ) {
    printf( "N" );
  } else {
    if( (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ) {
      printf( "D" );
    } else {
      printf( "#%d ", find_file_data.nFileSizeLow );
    }
    SYSTEMTIME file_system_time = { 0 };
    FileTimeToSystemTime(
      & find_file_data.ftLastWriteTime, & file_system_time
    );
    printf(
      "#%d #%d #%d #%d #%d #%d %s",
      file_system_time.wYear,
      file_system_time.wMonth,
      file_system_time.wDay,
      file_system_time.wHour,
      file_system_time.wMinute,
      file_system_time.wSecond,
      find_file_data.cFileName
    );
    FindClose( hFind );
  }
  return 0;
}
