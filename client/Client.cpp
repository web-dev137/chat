#include "Client.h"

Client::Client(){
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    inet_pton(AF_INET,this->BASE_IP,&serverAddress.sin_addr);
}

bool Client::connectClient() {
    // creating socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        // sending connection request
    SOCKET connectStatus = connect(clientSocket, (sockaddr*)&serverAddress,
            sizeof(sockaddr_in));
    if(connectStatus != 0) {
        return false;
    }
    return true;
}

std::string Client::formattingMsg(std::string name,std::string msg) {
    int posLast = msg.find_first_of("\n");
    msg = msg.substr(0,posLast);
    std::string chatMsg = name + ": " +msg+"\n";
    return chatMsg;
}
void Client::sendMsg(std::string msg){
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

std::string Client::recvMsg(){
    char buff[BUFF_SIZE];
    memset(buff, 0, sizeof(buff));
    int n=recv(clientSocket,buff,sizeof(buff),0);
    if(n<=0) {
        closesocket(clientSocket);
        return "";
    }
    std::lock_guard<std::mutex> lock(this->clientsMutex);
    return std::string(buff,n);
}

void Client::close() {
    // Закрытие сокета
   closesocket(clientSocket);
}