# TCP Chat

A simple multi-threaded client-server chat implemented in modern C++17 using raw sockets.

**Purpose of the project**: to deepen understanding of low-level network programming, thread synchronization, and client-server architecture.

## Key Features
- **Thread-per-client** concurrency model
- Message queues with synchronization via `std::mutex` + `std::condition_variable`
- Abstract transport layer (currently implemented over TCP sockets)
- Graceful client disconnection handling
- Simple text-based protocol

## Tech Stack
- C++17
- STL (threads, mutexes, condition variables, queues)
- TCP Sockets ( Winsock compatible)

## Build

```bash
git clone https://github.com/web-dev137/tcp-chat.git
cd tcp-chat

mkdir build && cd build

# Linux / macOS
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Windows (MinGW)
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
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

[User1]: Hi everyone!

[User2]: Hey, how are you?

[User1]: Doing well, thanks!

## Planned
```text
- Stress tests
- Non-blocking I/O
- Better concurrency model
```



