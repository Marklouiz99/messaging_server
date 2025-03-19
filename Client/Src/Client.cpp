#include"Client.h"

Client::Client(const std::string& ip, uint16_t port) : ip(ip), port(port), sock(INVALID_SOCKET)
{
}

Client::~Client()
{
    disconnect();
}

bool Client::connect()
{
    std::cout << "Connecting to " << ip << ":" << port << "\n";  
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << "\n";
        return false;
    }
     addrinfo hints = {};
     hints.ai_family = AF_INET;         
     hints.ai_socktype = SOCK_STREAM;   
     hints.ai_protocol = IPPROTO_TCP;
     std::string portStr = std::to_string(port);
     addrinfo* result = nullptr;
     int iResult = getaddrinfo(ip.c_str(), portStr.c_str(), &hints, &result);
     if (iResult != 0 || result == nullptr) {
         std::cerr << "getaddrinfo failed\n";
         WSACleanup();
         return false;
     }
 
     sock = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
     if (sock == INVALID_SOCKET) {
         std::cerr << "socket() failed\n";
         freeaddrinfo(result);
         WSACleanup();
         return false;
     }
 
     iResult = ::connect(sock, result->ai_addr, static_cast<int>(result->ai_addrlen));
     freeaddrinfo(result);
     if (iResult == SOCKET_ERROR) {
         std::cerr << "connect() failed\n";
         closesocket(sock);
         WSACleanup();
         return false;
     }
 
     std::cout << "Connected to the server!\n";
     return true;

}

bool Client::disconnect() {
    if (sock != INVALID_SOCKET) {
        shutdown(sock, SD_BOTH);
        closesocket(sock);
        sock = INVALID_SOCKET;
        std::cout << "Disconnected from the server!\n";
        WSACleanup();
    }
    return true;
}

bool Client::send(const messages::ClientMessage& message) {
    if (sock == INVALID_SOCKET) {
        std::cerr << "No active connection to send message.\n";
        return false;
    }
    std::string buffer;
    if (!message.SerializeToString(&buffer)) {
        std::cerr << "Failed to serialize ClientMessage.\n";
        return false;
    }
    int totalSent = 0;
    int toSend = static_cast<int>(buffer.size());
    while (totalSent < toSend) {
        int sent = ::send(sock, buffer.data() + totalSent, toSend - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "send() failed: " << WSAGetLastError() << "\n";
            return false;
        }
        totalSent += sent;
    }
    std::cout << "Sent message (" << totalSent << " bytes).\n";
    return true;
}

bool Client::receive(messages::ServerMessage& outMessage) {
    if (sock == INVALID_SOCKET) {
        std::cerr << "No active connection to receive message.\n";
        return false;
    }
    std::vector<char> buffer(1024);
    int bytesRead = ::recv(sock, buffer.data(), static_cast<int>(buffer.size()), 0);
    if (bytesRead == 0) {
        std::cerr << "Server disconnected.\n";
        return false;
    } else if (bytesRead < 0) {
        std::cerr << "recv() failed: " << WSAGetLastError() << "\n";
        return false;
    }
    std::cout << "Received " << bytesRead << " bytes from the server.\n";
    if (!outMessage.ParseFromArray(buffer.data(), bytesRead)) {
        std::cerr << "Failed to decode ServerMessage.\n";
        return false;
    }
    return true;
}