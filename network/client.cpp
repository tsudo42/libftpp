#include "network/client.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

Client::~Client()
{
    disconnect();
}

void Client::connect(const std::string &address, const size_t &port)
{
    disconnect();
    try
    {
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &addr_) != 0)
        {
            throw ClientException("Failed to get address info");
        }

        fd_ = socket(addr_->ai_family, addr_->ai_socktype, addr_->ai_protocol);
        if (fd_ == -1)
        {
            throw ClientException("Failed to create socket");
        }

        if (::connect(fd_, addr_->ai_addr, addr_->ai_addrlen) == -1)
        {
            throw ClientException("Failed to connect to server");
        }
    }
    catch (const ClientException &e)
    {
        disconnect();
        throw;
    }
}

void Client::disconnect()
{
    if (fd_ != -1)
    {
        close(fd_);
        fd_ = -1;
    }
    if (addr_ != nullptr)
    {
        freeaddrinfo(addr_);
        addr_ = nullptr;
    }
}

void Client::defineAction(const Message::Type &messageType,
                          const std::function<void(const Message &)> &action)
{
    actions_[messageType] = action;
}

void Client::defineAction(const Message::Type &messageType,
                          const std::function<void(Message &)> &action)
{
    actions_[messageType] = action;
}

void Client::send(const Message &message)
{
    if (fd_ == -1)
    {
        throw ClientException("Not connected to server");
    }

    if (Message::send(fd_, message, MSG_NOSIGNAL) < 0)
    {
        throw ClientException("Failed to send message");
    }
}

void Client::update()
{
    if (fd_ == -1)
    {
        return;
    }

    Message msg;
    while (Message::recv(fd_, msg, MSG_WAITALL | MSG_DONTWAIT) > 0)
    {
        auto action = actions_.find(msg.getType());
        if (action != actions_.end())
        {
            action->second(msg);
        }
    }
}

bool Client::isConnected() const
{
    return fd_ != -1;
}
