#include<iostream>
#include"server.h"


int main(int argc,const char **argv) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
    if(iResult != 0){
        printf("WSAStartup failed:%d",iResult);
        return false;
    }
    if(argc != 2) {
        std::cerr<<"Port not given"<<std::endl;
        exit(0);
    }
    int port = std::stoi(argv[1]);
    Server *s = new Server(port);
   // s->initServer();
    std::cout<<"Server waiting...";
    s->run();
    s->stop();
}