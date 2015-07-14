#include "clients_manager.h"

ClientsManager::ClientsManager() {

}
ClientsManager::~ClientsManager() {

}

const TCHAR ClientsManager::kMappingObjectName[19] = TEXT("MyFTPMappingObject");

LPCTSTR ClientsManager::CreateSharedMemory() {
  LPCTSTR pointer_to_buffor;

  handler_to_mapped_file_main_ = CreateFileMapping(
    INVALID_HANDLE_VALUE,    // use paging file
    NULL,                    // default security
    PAGE_READWRITE,          // read/write access
    0,                       // maximum object size (high-order DWORD)
    kBufforSize,                // maximum object size (low-order DWORD)
    kMappingObjectName);                 // name of mapping object

  if (handler_to_mapped_file_main_ == NULL) {
    _tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
    return 0;
  }
  pointer_to_buffor = (LPTSTR)MapViewOfFile(handler_to_mapped_file_main_,   // handle to map object
    FILE_MAP_ALL_ACCESS, // read/write permission
    0,
    0,
    kBufforSize);

  if (pointer_to_buffor == NULL) {
    _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
    CloseHandle(handler_to_mapped_file_main_);
    return 0;
  }
  // Write "0" to shared memory
  int transmitted_value = 0;
  _stprintf(shared_message_, TEXT("%d"), transmitted_value);
  CopyMemory((PVOID)pointer_to_buffor, shared_message_, (_tcslen(shared_message_) * sizeof(TCHAR)));

  return pointer_to_buffor;
}

LPCTSTR ClientsManager::MappedSharedMemory() {
  LPCTSTR pointer_to_buffor;

  handler_to_mapped_file_ = OpenFileMapping(
    FILE_MAP_ALL_ACCESS,   // read/write access
    FALSE,                 // do not inherit the name
    kMappingObjectName);               // name of mapping object

  if (handler_to_mapped_file_ == NULL) {
    _tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
    return 0;
  }
  pointer_to_buffor = (LPTSTR)MapViewOfFile(handler_to_mapped_file_,   // handle to map object
    FILE_MAP_ALL_ACCESS, // read/write permission
    0,
    0,
    kBufforSize);

  if (pointer_to_buffor == NULL) {
    _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
    CloseHandle(handler_to_mapped_file_);
    return 0;
  }

  return pointer_to_buffor;
}

int ClientsManager::ReadFromSharedMemory(LPCTSTR pointer_to_buffor) {
  if (pointer_to_buffor == NULL) {
    _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
    CloseHandle(handler_to_mapped_file_);
    return -1;
  }
  USES_CONVERSION;
  char *shared_as_text = W2A(pointer_to_buffor);
  int shared_as_int = atoi(shared_as_text);
  return shared_as_int;
}

void ClientsManager::WriteToSharedMemory(LPCTSTR pointer_to_buffor, int new_shared_int) {
  if (pointer_to_buffor == NULL) {
    _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
    CloseHandle(handler_to_mapped_file_);
    return;
  }
  _stprintf(shared_message_, TEXT("%d"), new_shared_int);
  CopyMemory((PVOID)pointer_to_buffor, shared_message_, (_tcslen(shared_message_) * sizeof(TCHAR)));
}

void ClientsManager::CloseMainSharedMemory(LPCTSTR pointer_to_buffor) {
  UnmapViewOfFile(pointer_to_buffor);
  CloseHandle(handler_to_mapped_file_main_);
}

void ClientsManager::CloseSharedMemory(LPCTSTR pointer_to_buffor) {
  UnmapViewOfFile(pointer_to_buffor);
  CloseHandle(handler_to_mapped_file_);
}