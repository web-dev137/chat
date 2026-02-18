#include "Client.h"

using namespace std;

std::string name;
std::string msg;

int main(int argc,const char **argv) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
    if(iResult != 0){
        printf("WSAStartup failed:%d",iResult);
        return false;
    }

    int port = std::stoi(argv[1]);

    std::unique_ptr<Client> c = std::make_unique<Client>(port);
    
     if(argc != 3) {
        std::cerr<<"Not all parameters are given"<<endl;
        exit(0);
    }

    name = "["+string(argv[2])+"]";
    // sending connection request
    SOCKET connectStatus = c->connectClient();
    if(!connectStatus) {
        std::cout<<"connect fail"<<WSAGetLastError();
        return false;
    }

    // sending data
    
    string msg;
    string clientBuffer;

    std::thread recivTh([&]() {
        while (true)
        {
            string msg = c->recvMsg();
            if(msg.empty()) {
                std::cout<<"server disconected";
                break;
            }
            std::cout<<msg;
        }
         c->close();   
    });
    recivTh.detach();    

    std::cout<<"Enter your message...\n";
    
    while (true)
    {
        std::getline(std::cin,msg);
        msg = c->formattingMsg(name,msg);
        c->sendMsg(msg);
    }
    cout<<"close socket";
    // closing socket
    c->close();
    return 0;
}