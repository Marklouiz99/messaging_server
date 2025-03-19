#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H 
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "D:\Mark\C++\MESSAGING_SERVER\proto\messages.pb.h" 

#pragma comment(lib, "Ws2_32.lib")

#endif

class ClientHandler
{
private:
    SOCKET clientSocket;
public:
    ClientHandler(SOCKET clientSocket);
    void handle();
    ~ClientHandler();
};