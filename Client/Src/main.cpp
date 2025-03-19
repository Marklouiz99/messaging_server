#include <iostream>
#include <cstdlib>
#include "Client.h"
#include "messages.pb.h"

int main(int argc, char* argv[]) {
    std::string ip = "127.0.0.1";
    uint16_t port = 8080;

    // Create a client instance
    Client client(ip, port);

    
    if (!client.connect()) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return EXIT_FAILURE;
    }

    messages::ClientMessage clientMsg;
    messages::EchoMessage* echoMsg = clientMsg.mutable_echo_message();
    echoMsg->set_content("Hello, Server!");


    if (!client.send(clientMsg)) {
        std::cerr << "Failed to send message." << std::endl;
        client.disconnect();
        return EXIT_FAILURE;
    }

    messages::ServerMessage serverMsg;
    if (!client.receive(serverMsg)) {
        std::cerr << "Failed to receive message." << std::endl;
        client.disconnect();
        return EXIT_FAILURE;
    }


    if (serverMsg.has_echo_message()) {
        std::cout << "Received echo message: " 
                  << serverMsg.echo_message().content() << std::endl;
    } else if (serverMsg.has_add_response()) {
        std::cout << "Received add response: " 
                  << serverMsg.add_response().result() << std::endl;
    } else {
        std::cout << "Received an unknown message." << std::endl;
    }

    
    client.disconnect();

    return EXIT_SUCCESS;
}

