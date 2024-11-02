#ifndef IO_MULTIPLEXER_HPP
#define IO_MULTIPLEXER_HPP

#include <vector>
#include <mutex>
#include <set>
#include <stdexcept>

class IOMultiplexer
{
public:
    virtual ~IOMultiplexer() = default;

    virtual bool addSocket(int socket_fd) = 0;
    virtual void removeSocket(int socket_fd) = 0;
    virtual std::vector<int> pollEvents() = 0;
    virtual void clearSocket() = 0;

    class IOMultiplexerError : public std::runtime_error
    {
    public:
        virtual ~IOMultiplexerError() = default;
        IOMultiplexerError(const std::string &msg);
    };
};

class SelectMultiplexer : public IOMultiplexer
{
public:
    SelectMultiplexer();
    ~SelectMultiplexer() override;

    bool addSocket(int socket_fd) override;
    void removeSocket(int socket_fd) override;
    std::vector<int> pollEvents() override;
    void clearSocket() override;

    int get_wakeup_fd() const noexcept;

private:
    std::mutex mutex_;
    std::set<int> sockets_;
    int wakeup_pipe_[2];
};

#endif /* IO_MULTIPLEXER_HPP */
