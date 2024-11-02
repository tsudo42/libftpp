#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstddef>
#include <functional>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include "network/message.hpp"

class Client
{
public:
    class ClientException;

    Client() = default;
    ~Client();

    void connect(const std::string &address, const std::size_t &port);
    void disconnect();

    void defineAction(const Message::Type &messageType,
                      const std::function<void(const Message &)> &action);
    void defineAction(const Message::Type &messageType,
                      const std::function<void(Message &)> &action);

    void send(const Message &message);
    void update();

    bool isConnected() const;

private:
    int fd_{-1};

    struct addrinfo *addr_{nullptr};

    std::unordered_map<Message::Type, std::function<void(Message &)>> actions_;
};

class Client::ClientException : public std::runtime_error
{
public:
    virtual ~ClientException() = default;
    ClientException(const std::string &msg) : std::runtime_error(msg) {}
};

#endif /* CLIENT_HPP */
