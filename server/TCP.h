#include<ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <stdint.h>
#include <thread>
#include <functional>
#include <iostream>
#include "ITransport.h"

class TCP:ITransport{
public:
    TCP(int port=5000);
    void send(int connectionId, const void* data) override;//отправка данных
    void start() override;//Запуск сервера
    void setMsgHandler(MsgHandler handler) override;//Установка 
    void setHandlerDisconnect(DisconnectHandler handler) override;
    void stop() override;
private:
    void bind();
    void listenOn();
    SOCKET handleAccept();
    void recv(SOCKET clientSocket);
 
    MsgHandler onMsg;//Обработчик сообщений для события получение клиентского сообщения
    DisconnectHandler onDisconnect;//Обработчик для случая разрыва соединения с сервером
    SOCKET sock;//
    SOCKET clientSocket;//
    sockaddr_in serverAddress;//
};