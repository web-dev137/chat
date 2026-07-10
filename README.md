## Chat on sockets

Chat - a simple multi-threaded client-server chat system implemented in c++.
The server uses a thread-per-client concurrency model with message queues and synchronization via mutex and condition_variable.
It features an abstract transport layer, currently implemented over TCP sockets(reciv,send,connect,accept), separating network handling from server logic.
This project demonstrates understanding of low-level network programming, thread synchronization, and client-server architecture.

## Install

In first you should clone project.
After this you need build project(from the root folder of the project):

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```
You see in build/client and build/server executable files(client.exe and server.exe).

## Run

For  run server:

    ./server.exe <port>

For connect client:

    ./client.exe <port> <client_name>

## Example work

```text
[client1]: hi
[client2]: hi
[client1]: how are you?
```
## Features

- Multi-threaded client-server architecture
- Thread-per-client dispatcher with message queue
- Abstract transport layer
- Graceful client disconnect handling

## Planned
```text
- Stress tests
- Non-blocking I/O
- Better concurrency model
```



