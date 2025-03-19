# MessagingServer

## Table of Contents
- [Project Description](#project-description)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building](#building)
- [Usage](#usage)
- [Troubleshooting](#troubleshooting)
- [Acknowledgements](#acknowledgements)
- [Contributing](#contributing)
- [License](#license)

---

## Project Description
**MessagingServer**A multi-threaded TCP messaging server and client built in C++ using Protocol Buffers for message serialization. This project demonstrates how to create a networked application that exchanges structured messages using modern C++ features (such as C++20’s std::jthread) and the Protocol Buffers library.

---
---
## Features
1. **Multi-threaded Server:**
Uses C++20 features (std::jthread) to accept and handle multiple client connections concurrently.

2.**Protocol Buffers Integration:**
Uses Protocol Buffers (protobuf) to serialize and deserialize messages between client and server.

3.**Message Types:**
The project supports multiple message types defined in a .proto file:

EchoMessage: Contains a simple string content.
AddRequest: Contains two integers.
AddResponse: Contains the result of adding two integers.
ClientMessage: A oneof field that can be either an EchoMessage or an AddRequest.
ServerMessage: A oneof field that can be either an EchoMessage or an AddResponse.

4.**Windows Networking (Winsock):**
Built using Winsock on Windows.

---

## Prerequisites
To build and run **MessagingServer**, you need the following:
- Windows and Windows Socket dependencies
- A C++20 compiler (GCC 11 or later recommended)
- Protocol Buffers Compiler and its dependencies (it's recommended to build the solution yourself using the source code)
- *(Optional)* Visual Studio Code for an enhanced development experience

---

---

## Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/Marklouiz99/messaging_server.git
   cd messaging_server
   ```
2. **Build or Install protocl buffer library and compiler using source code or packet manger such as Msys2**
3. **Compile the messages.proto**
   ```bash
   cd ./proto
    protoc --cpp_out=. messages.proto
   ```
---
---
## Building
1. **Using g++ and the terminal (Don't build the client side and server side together)
2. **Using tasks.json one for each client and server (if you build the server).

---
---
## Usage
Usage
1.**Run the Server:**
Execute the built server executable from the command line. The server will listen on port 8080 (or the port you specify).

2.**Connect a Client:**
Use a compatible client (for example, your own client code or a testing tool) that sends a ClientMessage (with either an EchoMessage or an AddRequest) serialized via Protocol Buffers.

3.**Receive Response:**
The server will process the message and send back a ServerMessage containing an echo reply or the sum of two numbers.

---

---

## Troubleshooting
1.**Undefined References / Linking Errors:**
Ensure that you link against the correct libraries (-lprotobuf, -lws2_32, and any required Abseil libraries if using a newer Protocol Buffers version).

2.**Include Path Issues:**
Verify that your IDE (or compiler) has the proper include paths to both your project headers and the Protocol Buffers runtime headers.

3.**Version Mismatch:**
Ensure that the version of protoc used to generate your code matches the runtime version you are linking against.

---

---
## Acknowledgements
1.**professor Nicholas Day videos about network programming on windows machine:** 
https://youtube.com/playlist?list=PL9HfA4ZKbzimKyvquT1MZ2x9d6UHjFNFA&si=2RpxbfnNMXoiHHNg
2.**Engineer Hussein Nasser protocol buffers crash course:**
https://youtu.be/46O73On0gyI?si=k5bIcu5rFJlawSZJ
---

---
## Contributing:

```
Contributions are welcome! Please fork the repository, make improvements, and open a pull request. For suggestions or bug reports, feel free to open an issue on GitHub.
```
---

---
## License

```
This project is licensed under the MIT License. See the LICENSE file for details.
```
---

---

