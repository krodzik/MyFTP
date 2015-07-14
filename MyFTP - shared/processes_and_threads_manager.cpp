#include "processes_and_threads_manager.h"

ProcessesAndThreadsManager::ProcessesAndThreadsManager() {
  ZeroMemory(&startup_info_, sizeof(startup_info_));
  startup_info_.cb = sizeof(startup_info_);
  ZeroMemory(&process_information_, sizeof(process_information_));
  path_to_process_ = new TCHAR[256];
}

ProcessesAndThreadsManager::~ProcessesAndThreadsManager() {

}

// not working currently
void ProcessesAndThreadsManager::SetPathToProcess(const char *path) {
  //printf("\n%s\n", path);

  //char *path_temp = new char[256];
  //strcpy(path_temp, path);
  
  // changing char* to TCHAR*
  USES_CONVERSION;
  path_to_process_ = A2T(path);

  //_tprintf(_T("Sciezka tuz przed wyslaniem: %s\n"), path_to_process_);
}

void ProcessesAndThreadsManager::CreateNewProcess() {
  // Start the child process.
  if (!CreateProcess(NULL,   // No module name (use command line)
    path_to_process_,        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    TRUE,          // Set handle inheritance to TRUE (trying to pass socket this way)   
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &startup_info_,            // Pointer to STARTUPINFO structure
    &process_information_)           // Pointer to PROCESS_INFORMATION structure
    )
  {
    printf("CreateProcess failed (%d).\n", GetLastError());
    return;
  }
}



void ProcessesAndThreadsManager::WaitForProcessToEnd() {
  WaitForSingleObject(process_information_.hProcess, INFINITE);
  printf("Proces sie zakonczyl.\n");
}
void ProcessesAndThreadsManager::CloseHandles() {
  CloseHandle(process_information_.hProcess);
  CloseHandle(process_information_.hThread);
}