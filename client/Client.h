#include<iostream>
#include<ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <mutex>
#include <thread>

class Client {
public:
    Client(int port);
    bool connectClient();
    void sendMsg(std::string msg);
    std::string formattingMsg(std::string name,std::string msg);
    std::string recvMsg();
    void close();
    std::mutex clientsMutex;
    const char* BASE_IP = "127.0.0.1";
private:
    
    SOCKET clientSocket;
    sockaddr_in serverAddress;
};