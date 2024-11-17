#include "network/server.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "network/endian.hpp"

Server::Server() : serverSocket_(-1), multiplexer_() {}

Server::~Server() { reset(); }

void Server::start(const std::size_t &p_port)
{
    if (running_.load())
    {
        return;
    }

    try
    {
        serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket_ == -1)
        {
            throw ServerException("Failed to create socket");
        }

        int opt = 1;
        if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        {
            throw ServerException("Failed to set socket options");
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(p_port);

        if (bind(serverSocket_, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        {
            throw ServerException("Failed to bind socket");
        }

        if (listen(serverSocket_, 5) == -1)
        {
            throw ServerException("Failed to listen on socket");
        }

        running_.store(true);
        multiplexer_.addSocket(serverSocket_);
        connectionThread_ = std::thread(&Server::connectionHandler, this);
    }
    catch (const ServerException &e)
    {
        reset();
        throw;
    }
}

void Server::defineAction(const Message::Type &messageType,
                          const std::function<void(ClientID &clientID, const Message &msg)> &action)
{
    std::lock_guard<std::mutex> lock(actions_mutex_);
    actions_[messageType] = action;
}

void Server::defineAction(const Message::Type &messageType,
                          const std::function<void(ClientID &clientID, Message &msg)> &action)
{
    std::lock_guard<std::mutex> lock(actions_mutex_);
    actions_[messageType] = action;
}

void Server::sendTo(const Message &message, ClientID clientID)
{
    ClientID id_to_release = -1;
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        auto client_iter = clients_.find(clientID);
        if (client_iter == clients_.end())
        {
            return;
        }
        std::shared_ptr<ConnectedClient> client = client_iter->second;
        if (client)
        {
            try
            {
                client->send(message);
            }
            catch (const ConnectedClient::ConnectionError &e)
            {
                id_to_release = client_iter->first.first;
            }
        }
    }
    if (id_to_release != -1)
    {
        releaseClient(id_to_release);
    }
}

void Server::sendToArray(const Message &message, const std::vector<ClientID> &clientIDs)
{
    for (ClientID clientID : clientIDs)
    {
        sendTo(message, clientID);
    }
}

void Server::sendToAll(const Message &message)
{
    std::vector<ClientID> ids_to_release;
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        for (auto &client_iter : clients_)
        {
            std::shared_ptr<ConnectedClient> client = client_iter.second;
            if (client)
            {
                try
                {
                    client->send(message);
                }
                catch (const ConnectedClient::ConnectionError &e)
                {
                    ids_to_release.push_back(client_iter.first.first);
                }
            }
        }
    }
    for (ClientID id : ids_to_release)
    {
        releaseClient(id);
    }
}

void Server::update()
{
    while (!messages_.empty())
    {
        auto [clientID, msg] = messages_.pop_front();
        std::function<void(ClientID & clientID, Message & msg)> action;
        {
            std::lock_guard<std::mutex> lock(actions_mutex_);
            if (actions_.find(msg.getType()) != actions_.end())
            {
                action = actions_[msg.getType()];
            }
        }
        if (action)
        {
            action(clientID, msg);
        }
    }
}

void Server::releaseClient(ClientID clientID)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);
    auto client_iter = clients_.find(clientID);
    if (client_iter == clients_.end())
    {
        return;
    }
    std::shared_ptr<ConnectedClient> client = client_iter->second;
    if (client)
    {
        multiplexer_.removeSocket(client->get_fd());
        clients_.erase(clientID);
    }
}

void Server::reset()
{
    running_.store(false);
    if (serverSocket_ != -1)
    {
        close(serverSocket_);
    }

    int wakeup_fd = multiplexer_.get_wakeup_fd();
    if (wakeup_fd > 0)
    {
        uint8_t byte = 1;
        write(wakeup_fd, &byte, 1);
    }

    if (connectionThread_.joinable())
    {
        try
        {
            connectionThread_.join();
        }
        catch (...)
        {
        }
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_.clear();
    }

    messages_.clear();

    {
        std::lock_guard<std::mutex> lock_actions(actions_mutex_);
        actions_.clear();
    }
}

void Server::connectionHandler() noexcept
{
    try
    {
        while (running_.load())
        {
            std::vector<ClientID> to_release;
            {
                std::vector<int> events = multiplexer_.pollEvents();
                if (running_.load() == false)
                {
                    break;
                }

                for (int fd : events)
                {
                    if (fd == serverSocket_)
                    {
                        struct sockaddr_in clientAddr;
                        socklen_t clientAddrSize = sizeof(clientAddr);
                        errno = 0;
                        int clientSocket = accept(serverSocket_, (struct sockaddr *)&clientAddr, &clientAddrSize);
                        if (clientSocket == -1)
                        {
                            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                            {
                                continue;
                            }
                            throw ServerException("Failed to accept connection");
                        }

                        if (!multiplexer_.addSocket(clientSocket))
                        {
                            close(clientSocket);
                            continue;
                        }

                        {
                            std::lock_guard<std::mutex> lock(clients_mutex_);
                            clients_.emplace(next_client_id, clientSocket, std::make_shared<ConnectedClient>(next_client_id, clientSocket, messages_));
                            next_client_id++;
                        }
                    }
                    else
                    {
                        std::lock_guard<std::mutex> lock(clients_mutex_);
                        auto client_iter = clients_.find(fd);
                        if (client_iter == clients_.end())
                        {
                            continue;
                        }
                        std::shared_ptr<ConnectedClient> client = client_iter->second;
                        if (client)
                        {
                            try
                            {
                                client->recv();
                            }
                            catch (const ConnectedClient::ConnectionError &e)
                            {
                                to_release.push_back(client_iter->first.first);
                            }
                        }
                    }
                }
            }
            for (ClientID id_to_release : to_release)
            {
                releaseClient(id_to_release);
            }
        }
    }
    catch (...)
    {
    }
    running_.store(false);
}

Server::ConnectedClient::~ConnectedClient() noexcept
{
    close(fd_);
}

void Server::ConnectedClient::recv()
{
    if (buffer_.size() - write_offset_ < readsize_)
    {
        buffer_.resize(write_offset_ + readsize_);
    }
    errno = 0;
    ssize_t received = ::recv(fd_, buffer_.data() + write_offset_, readsize_, 0);
    if (received <= 0)
    {
        throw ConnectionError("recv() failed");
    }
    write_offset_ += received;
    while (write_offset_ >= pending_bufsize_)
    {
        try
        {
            process();
        }
        catch (...)
        {
            throw ConnectionError("recieved malformed message");
        }
    }
}

void Server::ConnectedClient::send(const Message &message) const
{
    if (Message::send(fd_, message, MSG_NOSIGNAL) <= 0)
    {
        throw ConnectionError("send() failed");
    }
}

int Server::ConnectedClient::get_fd() const
{
    return fd_;
}

void Server::ConnectedClient::process()
{
    if (!read_size_ && write_offset_ >= sizeof(std::int64_t))
    {
        std::int64_t rawSize;
        std::memcpy(&rawSize, buffer_.data(), sizeof(rawSize));
        pending_bufsize_ = Ntoh(rawSize);

        buffer_.erase(buffer_.begin(), buffer_.begin() + sizeof(std::int64_t));
        write_offset_ -= sizeof(std::int64_t);
        read_size_ = true;
    }
    if (read_size_ && write_offset_ >= pending_bufsize_)
    {
        std::vector<std::byte> buffer(buffer_.begin(), buffer_.begin() + pending_bufsize_);
        Message msg;
        msg.deserialize(buffer_);

        msg_queue_.push_back(std::make_pair(id, msg));

        buffer_.erase(buffer_.begin(), buffer_.begin() + pending_bufsize_);
        write_offset_ -= pending_bufsize_;
        read_size_ = false;
        pending_bufsize_ = sizeof(std::int64_t);
    }
}
