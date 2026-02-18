#include "server.h"

Server::Server(int port):tcp(port)
{
    ITransport::MsgHandler cbMsg = [&](int clientSocket,const std::string& buf){
       this->handleRecive(clientSocket,buf);
    };
    ITransport::DisconnectHandler cbDisconect = [&](SOCKET clientSocket){
        this->deleteClientFromMap(clientSocket);
    };
    tcp.setMsgHandler(cbMsg);//Установка обработчика сообщений
    tcp.setHandlerDisconnect(cbDisconect);//Установка обработчика потери соединения клиента
}

/**
 * 
 */
void Server::run(){

    std::thread(&Server::handleSend, this).detach();//Поток отправки сообщений
    
    try {
        tcp.start();
    } catch(const std::runtime_error& e) {
        std::cerr<<"The server crashed with an runtime error:"<<e.what()<<std::endl;
    } catch(const std::exception& e) {
        std::cerr<<"The server crashed with errror:"<<e.what()<<std::endl;
    } catch(...) {
        std::cerr<<"Unknown error"<<std::endl;
    }
}

void Server::handleRecive(int clientSocket,const std::string& buff) {
    if(cln_sockets.find(clientSocket) == cln_sockets.end()){
        std::string clientName = parseClientName(buff);//Парсинг имени
        std::string errMsg = "Name already taken\n";
        
        /**
         * Проверка имени
         */
        if(!this->checkClientName(clientName)) {
            this->sendClientError(errMsg,clientSocket);
            return;
        };
        Client client = {clientName}; 
        this->cln_names.insert(clientName);
        this->addClientToMap(clientSocket,client);
        this->pushToChat(this->formattingRecivMsg(buff));//Отправка сообщения в очередь
    } else {
        std::string msg = this->formattingRecivMsg(buff);//Получаем сообщение
        this->pushToChat(msg);
    }
}

/**
 * Парсинг имени пользователя
 */
std::string Server::parseClientName(std::string clientBuff) {
    int posEndName = clientBuff.find_first_of(":");
    if(posEndName == std::string::npos) {
        throw std::invalid_argument("Invalid message format");
    }
    std::string clientName = clientBuff.substr(0,posEndName);
    return clientName;
}

/**
 * Форматирование полученного сообщения
 */
std::string Server::formattingRecivMsg(std::string clientBuff) {
    int endMsg = clientBuff.find_first_of("\n");
    clientBuff = clientBuff.substr(0,endMsg);
    clientBuff=clientBuff+"\n";
    return clientBuff;
}

/**
 * Обработка отправки сообщений
 */
void Server::handleSend() {

    while (running)
    {
        std::string msg;//
        msg = this->takeMsgFromQueue();//Взяли сообщение из очереди
        this->broadcast(msg);//Разослали сообщение остальным пользователям чата
    }
}

/**
 * Проверка имени пользователя на уникальность
 */
bool Server::checkClientName(std::string clientName) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    if(
        this->cln_names.find(clientName) != this->cln_names.end()  
        && !this->cln_names.empty()
    ) {
        return false;
    }

    return true;
}

/**
 * Поток для рассылки сообщения пользователям
 */
void Server::broadcast(std::string msg) {
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& [clientSocket,clientItem] : this->cln_sockets){
            send(clientSocket,msg.c_str(),msg.size(),0);
        }
    }
}

/**
 * Берём сообщение из очереди сообщений
 */
std::string Server::takeMsgFromQueue() {
    /**
     * Если очередь сообщений пуста ждём уведомление о новых
     * иначе берём сообщение из очереди 
     */
    {
        std::unique_lock<std::mutex> lock(this->chatMutex);//Блокировка потока чата
        if(this->chat.empty()){
            this->cv.wait(lock,[this]{return !this->chat.empty();});//Ждём пока в очередь не придёт сообщение
        }
        std::string msg = this->chat.front();//Извлекаем первое сообщение из очереди
        this->chat.pop();//Удаляем сообщение из очереди
        return msg;
    }
}

/**
 * Отправка ошибки пользователю
 */
void Server::sendClientError(std::string errMsg,SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(clientsMutex);//Блокировка потока клиентов
    send(clientSocket,errMsg.c_str(),errMsg.size(),0);
    this->deleteClientFromMap(clientSocket);
    closesocket(clientSocket);
}

/**
 * Добавляем пользователя в очередь активных соединений
 */
void Server::addClientToMap(SOCKET clientSocket,Client client){
    std::lock_guard<std::mutex> lock(clientsMutex);
    this->cln_sockets[clientSocket] = client;
}

/**
 * Удаляем пользователя из очереди активных соединений клиентов
 */
void Server::deleteClientFromMap(SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    this->cln_sockets.erase(clientSocket);
}

/**
 * Добавляем сообщение в очередь
 */
void Server::pushToChat(std::string msg) {
    {
        std::lock_guard<std::mutex> lock(this->chatMutex);
        this->chat.push(msg);
    }   
    this->cv.notify_one();//Уведомляем поток отправки сообщений
}



void Server::stop(){
    running = false;
    cv.notify_all();
    tcp.stop();
}