#ifndef MYFTP_SHARED_CLIENTS_MANAGER_H
#define MYFTP_SHARED_CLIENTS_MANAGER_H

#include <stdio.h>
// for A2T(), W2A()
#include "atlstr.h"

class ClientsManager {
public:
  //const, static
  //static int number_of_clients_per_process_;
  
  //konstruktor, destruktor
  ClientsManager();
  ~ClientsManager();
  //metody
  LPCTSTR CreateSharedMemory();
  LPCTSTR MappedSharedMemory();
  int ReadFromSharedMemory(LPCTSTR);
  void WriteToSharedMemory(LPCTSTR, int);
  void CloseMainSharedMemory(LPCTSTR);
  void CloseSharedMemory(LPCTSTR);
  //zmienne
protected:

private:
  const int kBufforSize = 8;
  static const TCHAR kMappingObjectName[19];// = TEXT("MyFTPMappingObject");

  TCHAR shared_message_[8];
  HANDLE handler_to_mapped_file_main_;
  HANDLE handler_to_mapped_file_;
};


#endif  // MYFTP_SHARED_CLIENTS_MANAGER_H