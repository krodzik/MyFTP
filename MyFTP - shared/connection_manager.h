#ifndef MYFTP_SHARED_CONNECTION_MANAGER_H_
#define MYFTP_SHARED_CONNECTION_MANAGER_H_

//do printf()
#include <stdio.h>
//do WinSocket
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class ConnectionManager {
public:
  //const, static
  //konstruktor, destruktor
  ConnectionManager();
  //metody
  SOCKET CreateServerConnection();
  SOCKET CreateClientConnection();
  SOCKET CreateSocket(struct addrinfo *);
  int BindSocket(SOCKET, struct addrinfo *);
  int ListenForClient(SOCKET);
  SOCKET AcceptingConnection(SOCKET);
  int ConnectToServer(SOCKET, struct addrinfo *);
  int ShutdownConnection(SOCKET);
  //zmienne
protected:

private:
  static const char *default_port_;
  //int socket_number_;

  int result_;
};


#endif  // MYFTP_SHARED_CONNECTION_MANAGER_H_