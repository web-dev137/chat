## Chat on sockets

Chat - a simple multi-threaded client-server chat system implemented in c++.
The server uses a thread-per-client concurrency model with message queues and synchronization via mutex and condition_variable.
It features an abstract transport layer, currently implemented over TCP sockets(reciv,send,connect,accept), separatting network handling from server logic.
This project demonstrates understanding of low-level network programing, thread synchronization, and client-server architecture.



## Concurrency model

The server use a thread-per-client model.
A dedicated acceptor thread waits for new client connections.
Each client connection spawns a worker thread that handles incoming data.
When data is received (recv), a callback processes it and pushes the message to the server’s message queue.
Sending messages is handled by taking items from the queue; if the queue is empty, the thread waits using condition_variable until a new message arrives.
Shared resources, such as the message queue and the list of clients, are protected using mutex.
This model works well for a small to medium number of clients. For larger loads, an event-driven or thread-pool-based architecture would be more scalable.

## Transport abstraction
The server features an abstract transport interface (ITransport), currently implemented with a TCP transport. This interface standardizes sending/receiving messages.
This allows potential extension to other protocols (TCP, UDP, etc.), without changing server logic.


## Install

In first you should clone project.
After this you need build project(from the root folder of the project):

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```
You see in builde/client and builder/server executable files(client.exe and server.exe).

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
- Better concurency model
```



