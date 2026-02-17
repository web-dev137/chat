#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <iostream>
#include <mutex>
#include <thread>
#include<ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>
#include <condition_variable>
#include "TCP.h"


#pragma comment(lib, "ws2_32.lib")

struct Client {
    std::string name;
};

class Server
{
public:
    Server(int port);
    void run();
    void stop();

private:
    TCP tcp;//TCP сервер
    std::condition_variable cv;//Условная переменная для уведомления потока отправки сообщений чтоо очередь сообщений не пуста
    std::unordered_map<SOCKET,Client> cln_sockets;//Очередь соединений клиентов
    std::unordered_set<std::string> cln_names;//Очередь имён
    std::queue<std::string> chat;//Очередь сообщений
    std::mutex clientsMutex;//Мьютексы для работы с пользователями
    std::mutex chatMutex;//Мьютексы для работы с сообщениями

    void handleRecive(int clientSocket,const std::string& buff);//Обработка получения сообщений
    void handleSend();//Обработка отправки сообщений
    void addClientToMap(SOCKET clientSocket, Client client);//Добавить пользователя в очередь активных соединений
    void deleteClientFromMap(SOCKET clientSocket);//Удалить пользователя из очереди соединений клиентов
    void sendClientError(std::string errMsg,SOCKET clientSocket);//Отправить ошибку клиенту
    void broadcast(std::string msg);//Рассылка сообщения пользователям чата
    bool checkClientSocket(int bytes,SOCKET clientSocket);
    bool checkClientName(std::string clientName);//Проверить имя на уникальность
    std::string takeMsgFromQueue();//Взять сообщение из очереди
    std::string parseClientName(std::string clientBuff);//Парсинг имени
    std::string formattingRecivMsg(std::string clientBuff);//Форматирование получаемых сообщений
    void pushToChat(std::string msg);//Отправка сообщений в очередь сообщений
};
#endif