// for getch()
#include <conio.h>
// for atoi()
#include <stdlib.h>
#include <string>
// WinSocket
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "clients_manager.h"

// do zamiany stringa na enuma
enum StringCommandEnum {
  kConnect,
  kUploadFile,
  kDownloadFile,
  kDownloadListOfFile,
  kDeleteFile,
  kRenameFile,
  kDisconnect,
  kHelp,
};

StringCommandEnum StringToEnumCommand(std::string const &in_string);

int main(int argc, char *argv[]) {
  // for socket to work 
  WSADATA wsaData;
  int result_ = 0;
  // Initialize Winsock
  result_ = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result_ != 0)
  {
    printf("WSAStartup failed: %d\n", result_);
    return 1;
  }

  // assign socket handle from process argument list passed by parent process/ server
  SOCKET client_socket = atoi(argv[1]);

#pragma region ODBIERANIE

#define DEFAULT_BUFLEN 512

  char recvbuf[DEFAULT_BUFLEN];
  int send_result;
  int recvbuflen = DEFAULT_BUFLEN;

  int result = 0;
  result = 0;

  // Receive until the peer shuts down the connection
  while ((result = recv(client_socket, recvbuf, recvbuflen, 0)) > 0) {
    recvbuf[result] = '\0';
    printf("Message: %s\n", recvbuf);

    switch (StringToEnumCommand(recvbuf)) {
    case kConnect:
      printf("Wybrales connect. Laczenie z serwerem.\n");
      break;
    case kUploadFile:
      printf("Wybrales upload file\n");
      break;
    case kDownloadFile:
      printf("Wybrales download file\n");
      break;
    case kDownloadListOfFile:
      printf("Wybrales download list of file \n");
      break;
    case kDeleteFile:
      printf("Wybrales del file\n");
      break;
    case kRenameFile:
      printf("Wybrales rename\n");
      break;
    case kDisconnect:
      printf("Wybrales disconnect\n");
      break;
    case kHelp:
      printf("Wybrales help\n");
      break;
    default:
      printf("Podales zla komende. Sprobuj jeszcze raz.\n");
    }

  }

#pragma endregion blok odpowiedzialny za przyjmowanie komunikatow od klientow

  WSACleanup();
  closesocket(client_socket);
  
  /*
#pragma region PAMIEC_WSPOLDZIELONA

  ClientsManager client;
  LPCTSTR pointer_to_buffer;
  pointer_to_buffer = client.MappedSharedMemory();
  int temp = 0;
  temp = client.ReadFromSharedMemory(pointer_to_buffer);
  printf("W tym momencie w buforze jest (w procesie) : %d\n", temp);
  client.WriteToSharedMemory(pointer_to_buffer, 23452);
  client.CloseSharedMemory(pointer_to_buffer);

#pragma endregion do komunikacji miêdzy procesami
  */
  
  return 0;
}

StringCommandEnum StringToEnumCommand(std::string const &in_string) {
  if (in_string == "connect") return kConnect;
  if (in_string == "upload") return kUploadFile;
  if (in_string == "download") return kDownloadFile;
  if (in_string == "downloadlist") return kDownloadListOfFile;
  if (in_string == "delete") return kDeleteFile;
  if (in_string == "rename") return kRenameFile;
  if (in_string == "disconnect") return kDisconnect;
  if (in_string == "help") return kHelp;
}