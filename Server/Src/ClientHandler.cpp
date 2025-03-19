#include"ClientHandler.h"

ClientHandler::ClientHandler(SOCKET clientSocket) : clientSocket(clientSocket)
{
   
}

ClientHandler::~ClientHandler()
{
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
    }
}

void ClientHandler::handle()
{
    char buffer[1024] = {0};
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            messages::ClientMessage clientMsg;
            if (clientMsg.ParseFromArray(buffer, bytesRead)) {
                std::cout << "Received client message." << std::endl;
                
                messages::ServerMessage serverMsg;
                // Determine which oneof field is set.
                switch (clientMsg.message_case()) {
                    case messages::ClientMessage::kEchoMessage: {
                        const std::string& text = clientMsg.echo_message().content();
                        std::cout << "Echo request: " << text << std::endl;
                        // Prepare a response echoing back the message.
                        messages::EchoMessage* echoResponse = serverMsg.mutable_echo_message();
                        echoResponse->set_content("Hello, " + text);
                        break;
                    }
                    case messages::ClientMessage::kAddRequest: {
                        int a = clientMsg.add_request().a();
                        int b = clientMsg.add_request().b();
                        std::cout << "Add request: " << a << " + " << b << std::endl;
                        // Compute result and prepare response.
                        messages::AddResponse* addResp = serverMsg.mutable_add_response();
                        addResp->set_result(a + b);
                        break;
                    }
                    default:
                        std::cerr << "Received unknown message type." << std::endl;
                        return;
                }

                // Serialize the response message.
                std::string out;
                if (serverMsg.SerializeToString(&out)) {
                    send(clientSocket, out.c_str(), static_cast<int>(out.size()), 0);
                    std::cout << "Response sent." << std::endl;
                } else {
                    std::cerr << "Failed to serialize server message." << std::endl;
                }
            } else {
                std::cerr << "Failed to parse client message." << std::endl;
            }
        } else {
            std::cerr << "Failed to read from client socket." << std::endl;
        }
}
