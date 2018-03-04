#include <windows.h>
#include <stdio.h>
#include <assert.h>

// Для Watcom (перед refalapi.h, после windows.h)
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "refalrts.h"
#include "refalapi.h"
#include "mr_oslib.h"

using refalapi::CharArray;
using refalapi::AllocArray;

/*===========================================================================
  Реализация функций библиотеки.
===========================================================================*/

DECL_REFAL_IDENT(Success, "Success");
DECL_REFAL_IDENT(Fails, "Fails");

// Функция, порождающая атрибутную информацию (объявление)
static bool add_attribute_record(
  AllocArray& allocs,
  const FILETIME& creation_time,
  const FILETIME& last_access_time,
  const FILETIME& last_write_time,
  DWORD file_attributes,
  DWORD size
);

REFAL_FUNC_IMPL(implement_fs::find_files) {
  /*
    Функция является оболочкой над парой WinAPI-функций
    FindFirstFile и FindNextFile.

    Формат функции:
      <FindFiles e.Mask>
        == #Success ((e.FileName) e.Attribs)*
        == #Fails

      e.Attribs = (s.Attribute e.Value)*

    s.Attribute может принимать следующие значения:
      * #ADateTime, #MDateTime и #CDateTime --- соответственно
        даты доступа, модификации и создания файла;
        формат e.Value:
          s.Year s.Month s.Day s.Hour s.Min s.Sec s.Millisec
      * #Size --- размер файла;
        формат e.Value:
          s.NumberSize | #Dir,
        соответственно размер файла в виде числа для регулярного файла
        или символ #Dir для папки.

    В дальнейшем возможно расширение набора атрибутов файла собственно
    собственно атрибутами: только для чтения, скрытый, архивный...
  */

  using namespace refalrts;

  // Скобки вызова, имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  // Сопоставление с образцом
  CharArray filemask;
  FnResult match = CharArray::from_sequence(filemask, arg_begin, arg_end);

  if( cSuccess != match ) return cRecognitionImpossible;

  // Построение списка каталогов в поле зрения

  WIN32_FIND_DATA find_file_data = { 0 };

  HANDLE hFind = FindFirstFile(filemask.c_str(), &find_file_data);

  if( hFind != INVALID_HANDLE_VALUE ) {
    // Открыли папку для просмотра, сканируем
    AllocArray allocs;

    reset_allocator();

    do {
      Iter open_main_bracket = 0, close_main_bracket = 0;
      Iter open_filename_bracket = 0, close_filename_bracket = 0;
      Iter filename_b = 0, filename_e = 0;

      bool allocated =
        alloc_open_bracket(open_main_bracket)
        && alloc_close_bracket(close_main_bracket)
        && alloc_open_bracket(open_filename_bracket)
        && alloc_close_bracket(close_filename_bracket)
        && alloc_string(filename_b, filename_e, find_file_data.cFileName);

      if( ! allocated ) return cRecognitionImpossible;

      link_brackets(open_main_bracket, close_main_bracket);
      link_brackets(open_filename_bracket, close_filename_bracket);

      allocs.push_back(open_main_bracket);
      allocs.push_back(open_filename_bracket);
      allocs.push_back(filename_b, filename_e);
      allocs.push_back(close_filename_bracket);

      allocated = add_attribute_record(
        allocs,
        find_file_data.ftCreationTime,
        find_file_data.ftLastAccessTime,
        find_file_data.ftLastWriteTime,
        find_file_data.dwFileAttributes,
        find_file_data.nFileSizeLow
      );

      if( ! allocated ) return cRecognitionImpossible;

      allocs.push_back(close_main_bracket);

    } while( FindNextFile(hFind, &find_file_data) != 0 );

    // Просмотр папки мог прерваться и ошибкой

    DWORD dwError = GetLastError();
    FindClose(hFind);

    if( ERROR_NO_MORE_FILES == dwError ) {
      // Переинициализируем open_call,
      // результат вставляем перед именем функции.
      open_call->link_info = 0;
      open_call->tag = cDataIdentifier;
      open_call->ident_info = REFAL_IDENT(Success);

      allocs.build( func_name );
      splice_to_freelist( func_name, close_call );
      return cSuccess;
    } else {
      // Чтение завершилось ошибкой
      // Переинициализация открывающей скобки
      open_call->link_info = 0;
      open_call->tag = cDataIdentifier;
      open_call->ident_info = REFAL_IDENT(Fails);

      splice_to_freelist( func_name, close_call );
      return cSuccess;
    }

  } else {
    // Открыть папку для просмотра не удалось
    // Переинициализируем открывающую угловую скобку
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Fails);

    splice_to_freelist(func_name, close_call);
    return cSuccess;
  }
}

DECL_REFAL_IDENT(ADateTime, "ADateTime");
DECL_REFAL_IDENT(CDateTime, "CDateTime");
DECL_REFAL_IDENT(MDateTime, "MDateTime");
DECL_REFAL_IDENT(Size, "Size");
DECL_REFAL_IDENT(Dir, "Dir");

static bool append_filetime(
  AllocArray& allocs,
  refalrts::RefalIdentifier ident,
  const FILETIME& time
);

// Функция, порождающая атрибутную информацию (определение)
static bool add_attribute_record(
  AllocArray& allocs,
  const FILETIME& creation_time,
  const FILETIME& last_access_time,
  const FILETIME& last_write_time,
  DWORD file_attributes,
  DWORD size
) {
  using namespace refalrts;

  bool append_time =
    append_filetime(allocs, REFAL_IDENT(ADateTime), last_access_time)
    && append_filetime(allocs, REFAL_IDENT(MDateTime), last_write_time)
    && append_filetime(allocs, REFAL_IDENT(CDateTime), creation_time);

  if( ! append_time ) return false;

  Iter open_bracket_size = 0, close_bracket_size = 0;
  Iter size_ident = 0, size_value = 0;

  bool is_directory = (file_attributes & FILE_ATTRIBUTE_DIRECTORY);
  bool size_allocs =
    alloc_open_bracket(open_bracket_size)
    && alloc_ident(size_ident, REFAL_IDENT(Size))
    &&
      (
        is_directory
        ? alloc_ident(size_value, REFAL_IDENT(Dir))
        : alloc_number(size_value, size)
      )
    && alloc_close_bracket(close_bracket_size);

  if( ! size_allocs ) return false;

  link_brackets(open_bracket_size, close_bracket_size);
  allocs.push_back(open_bracket_size);
  allocs.push_back(size_ident);
  allocs.push_back(size_value);
  allocs.push_back(close_bracket_size);

  return true;
}

static bool append_filetime(
  AllocArray& allocs,
  refalrts::RefalIdentifier ident,
  const FILETIME& time
) {
  using namespace refalrts;

  SYSTEMTIME file_system_time = { 0 };
  FileTimeToSystemTime( & time, & file_system_time );

  Iter open_bracket = 0, close_bracket = 0;
  Iter time_ident = 0;
  Iter year = 0;
  Iter month = 0;
  Iter day = 0;
  Iter hour = 0;
  Iter minute = 0;
  Iter second = 0;
  Iter milliseconds = 0;

  bool alloc_time =
    alloc_open_bracket( open_bracket )
    && alloc_close_bracket( close_bracket )
    && alloc_ident( time_ident, ident )
    && alloc_number( year, file_system_time.wYear )
    && alloc_number( month, file_system_time.wMonth )
    && alloc_number( day, file_system_time.wDay )
    && alloc_number( hour, file_system_time.wHour )
    && alloc_number( minute, file_system_time.wMinute )
    && alloc_number( second, file_system_time.wSecond )
    && alloc_number( milliseconds, file_system_time.wMilliseconds );

  if( ! alloc_time ) return false;

  link_brackets( open_bracket, close_bracket );
  allocs.push_back( open_bracket );
  allocs.push_back( time_ident );
  allocs.push_back( year );
  allocs.push_back( month );
  allocs.push_back( day );
  allocs.push_back( hour );
  allocs.push_back( minute );
  allocs.push_back( second );
  allocs.push_back( milliseconds );
  allocs.push_back( close_bracket );

  return true;
}

REFAL_FUNC_IMPL(implement_fs::file_attributes) {
  /*
    Функция получает имя файла и возвращает его атрибуты.

    Формат:
      <FileAttributes e.FileName>
        == #Success e.Attributes
        == #Fails

    e.Attributes имеет тот же формат, что и в функции FindFiles.
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left( arg_begin, arg_end );
  Iter func_name = arg_begin;
  move_left( arg_begin, arg_end );
  Iter close_call = arg_end;
  move_right( arg_begin, arg_end );

  CharArray file_name;
  FnResult match_name = CharArray::from_sequence(file_name, arg_begin, arg_end);

  if( cSuccess != match_name ) return cRecognitionImpossible;

  // Запрос атрибутов файла
  WIN32_FILE_ATTRIBUTE_DATA attributes = { 0 };
  BOOL get_attribute = GetFileAttributesEx(
    file_name.c_str(),
    GetFileExInfoStandard,
    & attributes
  );

  if( get_attribute ) {
    // Получение атрибутов удалось

    // Распределяем память для результата
    reset_allocator();

    AllocArray allocs;
    bool alloc_attribs = add_attribute_record(
      allocs,
      attributes.ftCreationTime,
      attributes.ftLastAccessTime,
      attributes.ftLastWriteTime,
      attributes.dwFileAttributes,
      attributes.nFileSizeLow
    );

    if( ! alloc_attribs ) return cNoMemory;

    // Переиспользуем открывающую угловую сбоку,
    // вставляем перед именем функции
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Success);

    allocs.build( func_name );
    splice_to_freelist( func_name, close_call );
    return cSuccess;
  } else {
    // Чтение атрибутов не удалось
    open_call->link_info = 0;
    open_call->tag = cDataIdentifier;
    open_call->ident_info = REFAL_IDENT(Fails);

    splice_to_freelist( func_name, close_call );
    return cSuccess;
  }
}

REFAL_FUNC_IMPL(implement_fs::full_name) {
  /*
    Возвращает абсолютный путь для относительного вроде как.

    Формат:
      <FullName e.RelativePath>
        == Success e.FullPath
        == Fails

    Функция не гарантирует того, что сформированный путь существует.
  */

  using namespace refalrts;

  // Угловые скобки и имя функции
  Iter open_call = arg_begin;
  move_left(arg_begin, arg_end);
  Iter func_name = arg_begin;
  move_left(arg_begin, arg_end);
  Iter close_call = arg_end;
  move_right(arg_begin, arg_end);

  CharArray relative_name;
  FnResult match_relative_name =
    CharArray::from_sequence(relative_name, arg_begin, arg_end);

  if( cSuccess != match_relative_name ) return match_relative_name;

  bool convert_ok;
  CharArray absolut_name;
  DWORD size = relative_name.strlen() + 1;

  for( ; ; ) {
    absolut_name.resize( size );
    char *p = 0;
    size = GetFullPathName(
      relative_name.c_str(),
      absolut_name.size(),
      absolut_name.c_str(),
      & p
    );

    /*
      Согласно документации на GetFullPathName, если имя файла успешно
      записано в предоставленный буфер, то функция возвращает длину имени
      исключая символ концевого нуля, т.е. size < absolut_name.size());
      если имя файла не влезает, то возвращается требуемая длина включая
      символ концевого нуля, т.е. size > absolut_name.size().
      Таким образом, возвращаемая длина никогда не будет равна размеру
      буфера.
    */
    if( (0 < size) && (size < absolut_name.size()) ) {
      // В размер буфера вписались, полный путь сформирован
      convert_ok = true;
      break;
    } else if( absolut_name.size() < size ) {
      // В размер буфера не вписались, увеличиваем его и повторяем
      continue;
    } else {
      assert( size == 0 );
      // Произошла ошибка формирования имени
      convert_ok = false;
      break;
    }
  }

  if( convert_ok ) {
    // Распределяем память для имени и помещаем его перед именем функции.
    reset_allocator();

    Iter eName_b = 0, eName_e = 0;
    if( ! alloc_string(eName_b, eName_e, absolut_name.c_str()) )
      return cNoMemory;

    splice_evar( func_name, eName_b, eName_e );
  }

  // Переиспользуем открывающую угловую скобку
  open_call->link_info = 0;
  open_call->tag = cDataIdentifier;
  open_call->ident_info =
    (convert_ok ? REFAL_IDENT(Success) : REFAL_IDENT(Fails));

  // Очиста поля зрения
  splice_to_freelist( func_name, close_call );
  return cSuccess;
}

refalrts::FnResult implement_fs::perform_file_operation(
  implement_fs::FileFunc operation,
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  /*
    Функция выполняет заданную операцию над заданным файлом.
    Функция является промежуточной для функций создания каталога,
    удаления файла и удаления каталога.

    Формат:
      <ФайловаяФункция e.FileName> == #Success | #Fails
  */

  using namespace refalrts;

  // Скобки вызова и имя функции
  Iter open_call = arg_begin;
  move_left( arg_begin, arg_end );
  Iter func_name = arg_begin;
  move_left( arg_begin, arg_end );
  Iter close_call = arg_end;
  move_right( arg_begin, arg_end );

  CharArray filename;
  FnResult match_filename = CharArray::from_sequence(filename, arg_begin, arg_end);

  if( cSuccess != match_filename ) return match_filename;

  bool result = operation(filename.c_str());

  // Переинициализируем открывающую угловую скобку
  open_call->link_info = 0;
  open_call->tag = cDataIdentifier;
  open_call->ident_info =
    (result ? REFAL_IDENT(Success) : REFAL_IDENT(Fails));

  splice_to_freelist(func_name, close_call);
  return cSuccess;
}

bool implement_fs::make_dir(const char *filename) {
  return CreateDirectory(filename, 0);
}

bool implement_fs::remove_dir(const char *filename) {
  return RemoveDirectory(filename);
}

bool implement_fs::remove_file(const char *filename) {
  return DeleteFile(filename);
}
