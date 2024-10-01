#ifndef MESSAGE_BUS_INTERFACE_H
#define MESSAGE_BUS_INTERFACE_H

#include <string>
#include <functional>


class MessageBusInterface
{
public:
    virtual void connect(const std::string& address) = 0;
    virtual void publish(const std::string& address, const std::string& message) = 0;
    virtual void subscribe(const std::string& address, std::function<void(const std::string&)> cb) = 0;
};

#endif //MESSAGE_BUS_INTERFACE_H
