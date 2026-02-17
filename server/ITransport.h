#include <stdint.h>

class ITransport {
public:
    using MsgHandler = std::function<void(int, const std::string&)>;//колбэк для обработки сообщений
    using DisconnectHandler = std::function<void(SOCKET)>;//
    virtual ~ITransport() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void setMsgHandler(MsgHandler handler) = 0;
    virtual void setHandlerDisconnect(DisconnectHandler handler) = 0;
    virtual void send(int connectionId, const void* data) = 0;
};