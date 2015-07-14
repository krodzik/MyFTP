// https://msdn.microsoft.com/en-us/library/windows/desktop/ms738545(v=vs.85).aspx
#include "connection_manager.h"

const char * ConnectionManager::default_port_ = "27015";

ConnectionManager::ConnectionManager() {
}

SOCKET ConnectionManager::CreateServerConnection() {
  WSADATA wsaData;

  // Initialize Winsock
  result_ = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result_ != 0)
  {
    printf("WSAStartup failed: %d\n", result_);
    return 1;
  }
  //---------------------------//

  struct addrinfo *ppresult = NULL, hints_about_socket;

  ZeroMemory(&hints_about_socket, sizeof(hints_about_socket));
  hints_about_socket.ai_family = AF_INET;
  hints_about_socket.ai_socktype = SOCK_STREAM;
  hints_about_socket.ai_protocol = IPPROTO_TCP;
  hints_about_socket.ai_flags = AI_PASSIVE;

  // Resolve the local address and port to be used by the server
  result_ = getaddrinfo(NULL, default_port_, &hints_about_socket, &ppresult);
  if (result_ != 0)
  {
    printf("getaddrinfo failed: %d\n", result_);
    WSACleanup();
    return 1;
  }

  // Create Socket
  SOCKET listen_socket;
  listen_socket = CreateSocket(ppresult);
  // Bind
  result_ = BindSocket(listen_socket, ppresult);
  // Address information returned by the getaddrinfo function is no longer needed. The freeaddrinfo() will free allocated memory
  freeaddrinfo(ppresult);
  // Listen
  result_ = ListenForClient(listen_socket);

  // Information about server (ip address, port, name)
  hostent *local_host;
  char *local_ip;
  local_host = gethostbyname("");
  local_ip = inet_ntoa(*reinterpret_cast<struct in_addr *>(*local_host->h_addr_list));
  printf("server: %s port %s\n", local_ip, default_port_); 
  char localhost[80];
  if (gethostname(localhost, sizeof(localhost)) == SOCKET_ERROR) {
    printf("Error %d when getting local host name.\n", WSAGetLastError());
    return 1;
  }
  printf("nazwa hosta: %s\n", localhost);

  // Accepting connection
  //SOCKET client_socket;
  //client_socket = AcceptingConnection(listen_socket);

  return listen_socket;
}

SOCKET ConnectionManager::CreateClientConnection() {
  WSADATA wsaData;

  // Initialize Winsock
  result_ = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result_ != 0)
  {
    printf("WSAStartup failed: %d\n", result_);
    return 1;
  }
  //---------------------------//

  struct addrinfo *ppresult = NULL, hints_about_socket;

  ZeroMemory(&hints_about_socket, sizeof(hints_about_socket));
  hints_about_socket.ai_family = AF_INET;
  hints_about_socket.ai_socktype = SOCK_STREAM;
  hints_about_socket.ai_protocol = IPPROTO_TCP;

  // Resolve the local address and port to be used by the server
  result_ = getaddrinfo(NULL, default_port_, &hints_about_socket, &ppresult);
  if (result_ != 0)
  {
    printf("getaddrinfo failed: %d\n", result_);
    WSACleanup();
    return 1;
  }

  // Create Socket
  SOCKET connect_socket;
  connect_socket = CreateSocket(ppresult);
  // Connect to server
  result_ = ConnectToServer(connect_socket, ppresult);

  return connect_socket;
}

SOCKET ConnectionManager::CreateSocket(struct addrinfo *ppresult) {
  // SOCKET create
  SOCKET listen_socket = INVALID_SOCKET;

  // Create a SOCKET for the server to listen for client connections
  listen_socket = socket(ppresult->ai_family, ppresult->ai_socktype, ppresult->ai_protocol);

  if (listen_socket == INVALID_SOCKET)
  {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(ppresult);
    WSACleanup();
    return 1;
  }
  return listen_socket;
}

int ConnectionManager::BindSocket(SOCKET listen_socket, struct addrinfo *ppresult) {
  // Setup the TCP listening socket
  result_ = bind(listen_socket, ppresult->ai_addr, static_cast<int>(ppresult->ai_addrlen));
  if (result_ == SOCKET_ERROR)
  {
    printf("bind failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(ppresult);
    closesocket(listen_socket);
    WSACleanup();
    return 1;
  }
  return 0;
}

int ConnectionManager::ListenForClient(SOCKET listen_socket) {
  //Listening on a Socket
  if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
  {
    printf("Listen failed with error: %ld\n", WSAGetLastError());
    closesocket(listen_socket);
    WSACleanup();
    return 1;
  }
  return 0;
}

SOCKET ConnectionManager::AcceptingConnection(SOCKET listen_socket) {
  SOCKET client_socket;
  struct sockaddr_in client_address;
  int client_length;

  //client_length = sizeof(client_address);
  // a zgodnie z google style
  client_length = sizeof(struct sockaddr_in);

  //client_socket = accept(listen_socket, NULL, NULL);
  client_socket = accept(listen_socket, reinterpret_cast<struct sockaddr *>(&client_address), &client_length);
  printf("server: got connection from %s port %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

  if (client_socket == INVALID_SOCKET)
  {
    printf("accept failed: %d\n", WSAGetLastError());
    closesocket(listen_socket);
    WSACleanup();
    return 1;
  }
  return client_socket;
}

int ConnectionManager::ConnectToServer(SOCKET connect_socket, struct addrinfo *ppresult) {
  result_ = connect(connect_socket, ppresult->ai_addr, static_cast<int>(ppresult->ai_addrlen));
  if (result_ == SOCKET_ERROR)	{
    closesocket(connect_socket);
    connect_socket = INVALID_SOCKET;
  }
  
  freeaddrinfo(ppresult);

  if (connect_socket == INVALID_SOCKET)	{
    printf("Unable to connect to server!\n");
    WSACleanup();
    return 1;
  }

  printf("Polaczylismy sie.\n");
  return 0;
}

int ConnectionManager::ShutdownConnection(SOCKET client_socket) {

  result_ = shutdown(client_socket, SD_SEND);
  if (result_ == SOCKET_ERROR) {
    printf("shutdown failed: %d\n", WSAGetLastError());
    closesocket(client_socket);
    WSACleanup();
    return 1;
  }

  // cleanup
  closesocket(client_socket);
  WSACleanup();

  return 0;
}