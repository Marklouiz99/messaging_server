#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include"ClientHandler.h"
#endif

class TCPServer
{ 
private:
    unsigned short port;
    SOCKET serverSocket;
    std::atomic<bool> running;
    std::jthread acceptThread;
    std::vector<std::jthread> clientThreads;
    void acceptConnections(std::stop_token token);
    void handleClient(SOCKET clientSocket, std::stop_token token);
public:
    TCPServer(unsigned short port);
    bool Start();
    void Stop();
    ~TCPServer();
};