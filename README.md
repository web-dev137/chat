#Chat on sockets

Chat is project allowing multiple connected users to communicate and exchange messages.
Currently, this is the initial version without rooms and moderation, but it is planned to expand the chat functionality in the future.

# Implementation

The software is divided into two separate programs, a server and a client project. The client-server architecture was developed using Winsock.

# Install

In first you should clone project.
After this you need build project(from the root folder of the project):

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```
You see in builde/client and builder/server executable files(client.exe and server.exe).

# Run

For  run server:

    ./server.exe <port>

For connect client:

    ./client.exe <port> <client_name>

## Planned
    - Chat rooms
    - Moderation



