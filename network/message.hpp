#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "data_structures/data_buffer.hpp"
#include <sys/types.h>

class Message : public DataBuffer
{
public:
    using Type = int;

    Message(Type type = 0);

    Message(const Message &other);
    Message &operator=(const Message &other);
    Message(Message &&other) noexcept;
    Message &operator=(Message &&other) noexcept;

    virtual ~Message() = default;

    void swap(Message &other) noexcept;

    Type getType() const;
    void setType(Type type);

    virtual std::vector<std::byte> serialize() const override;
    virtual void deserialize(const std::vector<std::byte> &buffer) override;

    static ssize_t send(int fd, const Message &message, int flags);
    static ssize_t recv(int fd, Message &message, int flags);

    class MessageException : public DataBufferException
    {
    public:
        virtual ~MessageException() = default;
        explicit MessageException(const std::string &msg);
    };

    class TypeOutOfRange : public MessageException
    {
    public:
        virtual ~TypeOutOfRange() = default;
        TypeOutOfRange();
    };

protected:
    Type type_;
};

#endif /* MESSAGE_HPP */
