#include"TCPServer.h"

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    TCPServer server(8080);
    if (!server.Start()) {
        std::cerr << "Failed to start server." << std::endl;
        return -1;
    }

    std::cout << "Press Enter to stop the server..." << std::endl;
    std::cin.get();

    server.Stop();

    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}