#include"TCPServer.h"

TCPServer::TCPServer(unsigned short port) : port(port), serverSocket(INVALID_SOCKET), running(false)
{
    
}

TCPServer::~TCPServer()
{
    Stop();
}

bool TCPServer::Start()
{
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed: " << wsaResult << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    BOOL opt = TRUE;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    running = true;
    std::cout << "Server started on port " << port<< std::endl;

    
    acceptThread = std::jthread([this](std::stop_token st) {
        this->acceptConnections(st);
    });
    
    return true;
}

void TCPServer::acceptConnections(std::stop_token stopToken) {
    while (!stopToken.stop_requested() && running) {
        sockaddr_in clientAddr = {};
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            if (running) {
                std::cerr << "Accept failed. Error: " << WSAGetLastError() << std::endl;
            }
            continue;
        }
        char clientIP[INET_ADDRSTRLEN] = { 0 };
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        std::cout << "Accepted connection from " << clientIP << std::endl;
        // Launch a jthread to handle the client.
        clientThreads.emplace_back([this, clientSocket](std::stop_token st) {
            this->handleClient(clientSocket, st);
        });       
    }
}

void TCPServer::handleClient(SOCKET clientSocket, std::stop_token stopToken) {
    ClientHandler handler(clientSocket);
        handler.handle();
}

void TCPServer::Stop() {
    if (running) {
        running = false;
        if (serverSocket!= INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }
        acceptThread.request_stop();
        for (auto &t : clientThreads) {
            t.request_stop();
        }
        WSACleanup();
    }
}