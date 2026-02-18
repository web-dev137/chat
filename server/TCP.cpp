#include "TCP.h"

TCP::TCP(int port) {
    serverAddress = {};
    inet_pton(AF_INET,"127.0.0.1",&serverAddress.sin_addr);
    serverAddress.sin_port = htons(port);
    serverAddress.sin_family = AF_INET;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }
    this->bind();//
    this->listenOn();

}

/**
 * Запуск сервера
 */
void TCP::start() {
    while(true) {
        clientSocket = this->handleAccept();
        if (clientSocket == INVALID_SOCKET){
            if(onDisconnect) {
                onDisconnect(clientSocket);
            }
            throw std::runtime_error("Failed to accept socket");
        }
        std::cout<<"Client connected";
        std::thread th(&TCP::recv,this,clientSocket);//Поток получения данных
        th.detach();
    }
}

/**
 * Установка обработчика сообщений
 */
void TCP::setMsgHandler(MsgHandler handler) {
    onMsg = handler;
}

/**
 * Установка  обработчика разрыва соединения
 */
void TCP::setHandlerDisconnect(DisconnectHandler handler) {
    onDisconnect = handler;
}

/**
 * Присваивание параметров сокету
 */
void TCP::bind() {
    int bindFlag = ::bind(sock,(struct sockaddr*)&serverAddress,sizeof(sockaddr_in));

    if(bindFlag == SOCKET_ERROR) {
        if(onDisconnect) {
            onDisconnect(clientSocket);
        }
        closesocket(sock);
        throw std::runtime_error("Failed bind params");
    }
}


SOCKET TCP::handleAccept() {

    SOCKET clientSocketID =  accept(sock,NULL,NULL);//Получаем клиентский сокет
   
    if (clientSocketID == INVALID_SOCKET){
         /**
         * Если данные не пришли то вызываем обработчик события 
         */
        if(onDisconnect) {
            onDisconnect(clientSocket);
        }
        throw std::runtime_error("Failed accept on invalid socket");
    } 

   return clientSocketID;
}

/**
 * Прослушивание серверного сокета
 */
void TCP::listenOn() {
    if(listen(sock,5) == SOCKET_ERROR) {
        closesocket(sock);
        throw std::runtime_error("Failed listening on socket");
    }
    
    std::cout<<"listening ok\n";
}

/**
 * Получение данных со стороны клиента
 */
void TCP::recv(SOCKET clientSocket) {
    char buffer[1024] = { 0 };//Клиентский буфер
  
    while(true){
        int n = ::recv(clientSocket ,buffer,sizeof(buffer),0); //Запись данных в буфер

        /**
         * Если данные не пришли то вызываем обработчик события 
         * и закрываем сокет
         */
        if(n<=0) {
            if(onDisconnect) {
                onDisconnect(clientSocket);
            }
            closesocket(clientSocket);
            throw std::runtime_error("Failed reciv data");
        }
        
        buffer[n] = '\0';
        
        /**
         * Если сообщение пришло то отправляем данные серверу чата для обработки
         */
        if(onMsg) {
            onMsg(clientSocket,std::string(buffer,n));
        }
    }
}

/**
 * Отправка данных клиенту
 */
void TCP::send(int connectionId,const void* data) {
    const char* buf = static_cast<const char*>(data); //приводим данные к типу необходимого для отправки
    ::send(connectionId,buf,strlen(buf),0); //отправка данных
}

/**
 * Остановка сервера
 */
void TCP::stop() {
    std::cout<<"Server shutting down...\n";
    closesocket(sock);
    WSACleanup(); 
}