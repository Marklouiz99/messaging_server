#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>

#include "messages.pb.h"

#pragma comment(lib, "Ws2_32.lib")

#endif // CLIENT_H

class Client {
private:
    std::string ip;
    uint16_t port;
    SOCKET sock;
public:
    Client(const std::string& ip, uint16_t port);
    ~Client() ;
    bool connect(); 
    bool disconnect(); 
    bool send(const messages::ClientMessage& message) ;
    bool receive(messages::ServerMessage& outMessage) ;

};
