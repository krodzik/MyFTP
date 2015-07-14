#ifndef MYFTP_SHARED_PROCESSES_AND_THREADS_MANAGER_H
#define MYFTP_SHARED_PROCESSES_AND_THREADS_MANAGER_H

//do printf()
#include <stdio.h>
// for A2T(), W2A()
#include "atlstr.h"
// for ZeroMmeory()
#include <Windows.h>
#include <tchar.h>


class ProcessesAndThreadsManager {
public:
  //const, static
  //konstruktor, destruktor
  ProcessesAndThreadsManager();
  ~ProcessesAndThreadsManager();
  //metody
  void SetPathToProcess(const char *);
  void CreateNewProcess();
  void WaitForProcessToEnd();
  void CloseHandles();
  //zmienne
  TCHAR *path_to_process_;
protected:
private:
  STARTUPINFO startup_info_;
  PROCESS_INFORMATION process_information_;
};

#endif  // MYFTP_SHARED_PROCESSES_AND_THREADS_MANAGER_H