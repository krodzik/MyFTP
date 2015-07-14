//do getch
#include <conio.h>

#include "processes_and_threads_manager.h"
#include "connection_manager.h"

int main(int argc, char *argv[])
{
  ConnectionManager connection;
  SOCKET connect_socket = connection.CreateClientConnection();

  #pragma region WYSYLANIE

  #define DEFAULT_BUFLEN 512

  int recvbuflen = DEFAULT_BUFLEN;

  char *sendbuf = new char[DEFAULT_BUFLEN];
  char recvbuf[DEFAULT_BUFLEN];

  int result = 0;

  do {
  printf("Wiadomosc do wyslania: \n");
  scanf("%s", sendbuf);
  //printf("Wiadomosc jaka wysle: %s\n", sendbuf);

  if (strcmp(sendbuf, "quit") == 0) break;
  // Send an initial buffer
  result = send(connect_socket, sendbuf, static_cast<int>(strlen(sendbuf)), 0);
  if (result == SOCKET_ERROR) {
  printf("send failed: %d\n", WSAGetLastError());
  closesocket(connect_socket);
  WSACleanup();
  //
  getch();
  //
  return 1;
  }
  } while (true);

  #pragma endregion blok odpowiedzialny za komunikacje z serwerem

  connection.ShutdownConnection(connect_socket);

	printf("Client exit.\n");
	getch();
	return 0;
}