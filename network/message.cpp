#include "message.hpp"
#include <limits>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

Message::Message(Type type) : DataBuffer(), type_(type) {}

Message::Message(const Message &other)
    : DataBuffer(other), type_(other.type_) {}

Message &Message::operator=(const Message &other)
{
    if (this != &other)
    {
        DataBuffer::operator=(other);
        type_ = other.type_;
    }
    return *this;
}

Message::Message(Message &&other) noexcept
    : DataBuffer(std::move(other)), type_(other.type_)
{
    other.type_ = 0;
}

Message &Message::operator=(Message &&other) noexcept
{
    if (this != &other)
    {
        DataBuffer::operator=(std::move(other));
        type_ = other.type_;
        other.type_ = 0;
    }
    return *this;
}

void Message::swap(Message &other) noexcept
{
    DataBuffer::swap(other);
    std::swap(type_, other.type_);
}

Message::Type Message::getType() const { return type_; }
void Message::setType(Type type) { type_ = type; }

std::vector<std::byte> Message::serialize() const
{
    std::vector<std::byte> buffer;
    buffer.resize(sizeof(std::int64_t) + body_.size());

    std::int64_t messageType = static_cast<std::int64_t>(type_);
    std::memcpy(buffer.data(), &messageType, sizeof(std::int64_t));
    std::memcpy(buffer.data() + sizeof(std::int64_t), body_.data(), body_.size());

    return buffer;
}

void Message::deserialize(const std::vector<std::byte> &buffer)
{
    if (buffer.size() <= sizeof(std::int64_t))
    {
        throw MessageException("Buffer too small to contain Message");
    }

    std::int64_t messageType;
    std::memcpy(&messageType, buffer.data(), sizeof(std::int64_t));

    if (messageType > std::numeric_limits<int>::max() || messageType < std::numeric_limits<int>::min())
    {
        throw TypeOutOfRange();
    }
    type_ = static_cast<int>(messageType);

    body_.resize(buffer.size() - sizeof(std::int64_t));
    std::memcpy(body_.data(), buffer.data() + sizeof(std::int64_t), buffer.size() - sizeof(std::int64_t));

    read_pos_ = 0;
}

ssize_t Message::send(int fd, const Message &message, int flags)
{
    std::vector<std::byte> buffer = message.serialize();

    std::int64_t size_raw = htonll(buffer.size());
    ssize_t sent_size = ::send(fd, &size_raw, sizeof(size_raw), flags);
    if (sent_size <= 0)
    {
        return sent_size;
    }
    ssize_t sent_body = ::send(fd, buffer.data(), buffer.size(), flags);
    if (sent_body <= 0)
    {
        return sent_body;
    }
    return sent_size + sent_body;
}

ssize_t Message::recv(int fd, Message &message, int flags)
{
    std::int64_t size_raw;
    ssize_t received_size = ::recv(fd, &size_raw, sizeof(size_raw), flags);
    if (received_size <= 0)
    {
        return received_size;
    }

    size_raw = ntohll(size_raw);
    std::vector<std::byte> buffer;
    buffer.resize(size_raw);
    ssize_t received_body = ::recv(fd, buffer.data(), buffer.size(), flags);
    if (received_body <= 0)
    {
        return received_body;
    }

    message.deserialize(buffer);
    return received_size + received_body;
}

Message::MessageException::MessageException(const std::string &msg)
    : DataBufferException(msg) {}
Message::TypeOutOfRange::TypeOutOfRange()
    : MessageException("Message type out of range") {}
