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
    if (read_pos_ > body_.size())
    {
        return std::vector<std::byte>();
    }
    std::vector<std::byte> buffer;
    buffer.resize(body_.size() - read_pos_);
    std::memcpy(buffer.data(), body_.data() + read_pos_, body_.size() - read_pos_);
    return buffer;
}

void DataBuffer::deserialize(const std::vector<std::byte> &buffer)
{
    body_ = buffer;
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
