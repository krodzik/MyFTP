//do getch
#include <conio.h>

#include "connection_manager.h"
#include "processes_and_threads_manager.h"

int main(int argc, char *argv[])
{
  ConnectionManager connection;
  SOCKET listen_socket = connection.CreateServerConnection();
  SOCKET client_socket;
  
  ProcessesAndThreadsManager process_manager;

  while (client_socket = connection.AcceptingConnection(listen_socket)) {

    // in there should be process_manager.SetPathToProcess function
    char *sciezka = new char[256];
    strcpy(sciezka, "F:\\dropbox\\myftp\\MyFTP - ClientsManager\\ClientsManager\\debug\\ClientsManager");
    strcat(sciezka, " ");
    // copy socket handle to argument list of new process
    char str_to_socket[10];
    itoa(client_socket, str_to_socket, 10);
    strcat(sciezka, str_to_socket);
    printf("Sciezka do procesu tuz przed wyslaniem:\n%s\n", sciezka);
    USES_CONVERSION;
    TCHAR* dziecko = A2T(sciezka);
    process_manager.path_to_process_ = dziecko;

    process_manager.CreateNewProcess();

  }

  process_manager.WaitForProcessToEnd();
  process_manager.CloseHandles();
  printf("Client disconnect.\n");

  connection.ShutdownConnection(client_socket);
  closesocket(listen_socket);

	printf("Server exit.\n");
	return 0;
}