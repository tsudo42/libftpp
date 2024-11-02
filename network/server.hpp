#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <unordered_map>
#include <functional>

#include "network/message.hpp"
#include "network/io_multiplexer.hpp"
#include "data_structures/dual_key_map.hpp"
#include "threading/thread_safe_queue.hpp"

class Server
{
public:
    typedef long long ClientID;

    class ConnectedClient;
    class ServerException;

    Server();
    ~Server();

    void start(const std::size_t &p_port);

    void defineAction(const Message::Type &messageType,
                      const std::function<void(ClientID &clientID, const Message &msg)> &action);
    void defineAction(const Message::Type &messageType,
                      const std::function<void(ClientID &clientID, Message &msg)> &action);

    void sendTo(const Message &message, ClientID clientID);
    void sendToArray(const Message &message, const std::vector<ClientID> &clientIDs);
    void sendToAll(const Message &message);

    void update();

    void releaseClient(ClientID clientID);

    void reset();

private:
    void acceptClients() noexcept;
    void clientHandler() noexcept;

    std::atomic<bool> running_{false};

    int serverSocket_;
    std::thread acceptThread_;
    std::thread handlerThread_;

    SelectMultiplexer multiplexer_;

    std::mutex clients_mutex_;
    ClientID next_client_id{1};
    DualKeyMap<ClientID, int, std::shared_ptr<ConnectedClient>> clients_;

    ThreadSafeQueue<std::pair<ClientID, Message>> messages_;

    std::mutex actions_mutex_;
    std::unordered_map<Message::Type, std::function<void(ClientID &clientID, Message &msg)>> actions_;
};

class Server::ConnectedClient
{
public:
    class ConnectionError;

    ConnectedClient() = delete;
    ConnectedClient(ClientID id, int fd, ThreadSafeQueue<std::pair<ClientID, Message>> &msg_queue) noexcept
        : id(id), fd_(fd), msg_queue_(msg_queue) {}

    ~ConnectedClient() noexcept;

    void recv() noexcept(false);
    void send(const Message &message) const noexcept(false);

    int get_fd() const;

private:
    void process();

    ClientID id;
    int fd_;
    ThreadSafeQueue<std::pair<ClientID, Message>> &msg_queue_;

    static const std::size_t readsize_{1024};

    std::vector<std::byte> buffer_{readsize_};
    std::size_t write_offset_{0};

    std::size_t pending_bufsize_{sizeof(std::int64_t)};
    bool read_size_{false};
};

class Server::ServerException : public std::runtime_error
{
public:
    virtual ~ServerException() = default;
    ServerException(const std::string &msg) : std::runtime_error(msg) {}
};

class Server::ConnectedClient::ConnectionError : public Server::ServerException
{
public:
    virtual ~ConnectionError() = default;
    ConnectionError(const std::string &msg) : Server::ServerException(msg) {};
};

#endif /* SERVER_HPP */
