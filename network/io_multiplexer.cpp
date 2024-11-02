#include "network/io_multiplexer.hpp"

#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

IOMultiplexer::IOMultiplexerError::IOMultiplexerError(const std::string &msg)
    : std::runtime_error(msg) {}

SelectMultiplexer::SelectMultiplexer()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pipe(wakeup_pipe_) == -1)
    {
        throw IOMultiplexerError("Failed to create wakeup pipe");
    }

    int flags = fcntl(wakeup_pipe_[0], F_GETFL, 0);
    if (flags == -1)
    {
        throw IOMultiplexerError("Failed to get flags for wakeup pipe");
    }
    if (fcntl(wakeup_pipe_[0], F_SETFL, flags | O_NONBLOCK) == -1)
    {
        throw IOMultiplexerError("Failed to set flags for wakeup pipe");
    }
    sockets_.insert(wakeup_pipe_[0]);
}

SelectMultiplexer::~SelectMultiplexer()
{
    close(wakeup_pipe_[0]);
    close(wakeup_pipe_[1]);
}

bool SelectMultiplexer::addSocket(int socket_fd)
{
    if (socket_fd < 0 || socket_fd >= FD_SETSIZE)
    {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto result = sockets_.insert(socket_fd);
        if (!result.second)
        {
            return false;
        }
    }

    uint8_t byte = 1;
    write(wakeup_pipe_[1], &byte, 1);

    return true;
}

void SelectMultiplexer::removeSocket(int socket_fd)
{
    if (socket_fd < 0 || socket_fd >= FD_SETSIZE)
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        sockets_.erase(socket_fd);
    }

    uint8_t byte = 1;
    write(wakeup_pipe_[1], &byte, 1);
}

std::vector<int> SelectMultiplexer::pollEvents()
{
    fd_set readfds, writefds, errorfds;
    int max_fd = -1;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&errorfds);

        for (const int fd : sockets_)
        {
            FD_SET(fd, &readfds);
            FD_SET(fd, &errorfds);
            if (fd > max_fd)
            {
                max_fd = fd;
            }
        }
    }
    FD_SET(wakeup_pipe_[0], &readfds);

    struct timeval timeout
    {
        60, 0
    };
    int retval;
    while (true)
    {
        errno = 0;
        retval = select(max_fd + 1, &readfds, &writefds, &errorfds, &timeout);
        if (retval == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                throw IOMultiplexerError("select() failed");
            }
        }
        break;
    }

    std::vector<int> events;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const int fd : sockets_)
        {
            if (FD_ISSET(fd, &readfds) || FD_ISSET(fd, &writefds) || FD_ISSET(fd, &errorfds))
            {
                if (fd == wakeup_pipe_[0])
                {
                    uint8_t byte;
                    while (read(wakeup_pipe_[0], &byte, 1) > 0)
                    {
                    }
                    continue;
                }
                events.push_back(fd);
            }
        }
    }

    return events;
}

void SelectMultiplexer::clearSocket()
{
    std::set<int> new_sockets;
    new_sockets.insert(wakeup_pipe_[0]);

    std::lock_guard<std::mutex> lock(mutex_);
    sockets_.swap(new_sockets);
}

int SelectMultiplexer::get_wakeup_fd() const noexcept
{
    return wakeup_pipe_[1];
}
