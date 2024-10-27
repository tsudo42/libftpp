#include "data_structures/data_buffer.hpp"
#include <limits>

DataBuffer::DataBuffer() : read_pos_(0) {}

DataBuffer::DataBuffer(const DataBuffer &other)
    : body_(other.body_), read_pos_(other.read_pos_) {}

DataBuffer::DataBuffer(DataBuffer &&other) noexcept
    : body_(std::move(other.body_)), read_pos_(other.read_pos_)
{
    other.read_pos_ = 0;
}

DataBuffer &DataBuffer::operator=(const DataBuffer &other)
{
    if (this != &other)
    {
        body_ = other.body_;
        read_pos_ = other.read_pos_;
    }
    return *this;
}

DataBuffer &DataBuffer::operator=(DataBuffer &&other) noexcept
{
    if (this != &other)
    {
        body_ = std::move(other.body_);
        read_pos_ = other.read_pos_;
        other.read_pos_ = 0;
    }
    return *this;
}

void DataBuffer::swap(DataBuffer &other) noexcept
{
    body_.swap(other.body_);
    std::swap(read_pos_, other.read_pos_);
}

DataBuffer &DataBuffer::operator<<(const std::string &data)
{
    size_t size = data.size();
    *this << size;
    size_t currentSize = body_.size();
    body_.resize(currentSize + size);
    std::memcpy(body_.data() + currentSize, data.data(), size);
    return *this;
}

DataBuffer &DataBuffer::operator>>(std::string &data)
{
    size_t size;
    *this >> size;
    if (read_pos_ + size > body_.size())
    {
        throw BodyUnderflow();
    }
    data.resize(size);
    std::memcpy(&data[0], body_.data() + read_pos_, size);
    read_pos_ += size;
    return *this;
}

void DataBuffer::clear()
{
    body_.clear();
    read_pos_ = 0;
}

std::vector<std::byte> DataBuffer::serialize() const
{
    std::vector<std::byte> buffer;
    buffer.resize(sizeof(int64_t) + body_.size());

    size_t offset = 0;

    int64_t bodySizeNetwork = static_cast<int64_t>(body_.size());
    std::memcpy(buffer.data() + offset, &bodySizeNetwork, sizeof(int64_t));
    offset += sizeof(int64_t);

    std::memcpy(buffer.data() + offset, body_.data(), body_.size());

    return buffer;
}

void DataBuffer::deserialize(const std::vector<std::byte> &buffer)
{
    if (buffer.size() < sizeof(int64_t))
    {
        throw InvalidBuffer("Buffer too small to contain DataBuffer");
    }

    size_t offset = 0;

    int64_t bodySizeNetwork;
    std::memcpy(&bodySizeNetwork, buffer.data() + offset, sizeof(int64_t));
    offset += sizeof(int64_t);

    if (bodySizeNetwork < 0 || static_cast<uint64_t>(bodySizeNetwork) > std::numeric_limits<std::size_t>::max())
    {
        throw BodySizeOutOfRange();
    }
    std::size_t bodySize = static_cast<std::size_t>(bodySizeNetwork);

    if (offset + bodySize > buffer.size())
    {
        throw InvalidBuffer("Buffer does not contain full DataBuffer body");
    }

    body_.resize(bodySize);
    std::memcpy(body_.data(), buffer.data() + offset, bodySize);

    read_pos_ = 0;
}

DataBuffer::DataBufferException::DataBufferException(const std::string &msg)
    : std::runtime_error(msg) {}

DataBuffer::BodyUnderflow::BodyUnderflow()
    : DataBufferException("DataBuffer body underflow") {}

DataBuffer::BodySizeOutOfRange::BodySizeOutOfRange()
    : DataBufferException("DataBuffer body size out of range") {}

DataBuffer::InvalidBuffer::InvalidBuffer(const std::string &msg)
    : DataBufferException(msg) {}
