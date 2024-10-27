#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <vector>
#include <string>
#include <type_traits>
#include <cstring>
#include <stdexcept>
#include <cstddef>

class DataBuffer
{
public:
    DataBuffer();

    DataBuffer(const DataBuffer &other);
    DataBuffer &operator=(const DataBuffer &other);
    DataBuffer(DataBuffer &&other) noexcept;
    DataBuffer &operator=(DataBuffer &&other) noexcept;

    virtual ~DataBuffer() = default;

    void swap(DataBuffer &other) noexcept;

    template <typename TType>
    DataBuffer &operator<<(const TType &data)
    {
        static_assert(std::is_trivially_copyable<TType>::value, "TType must be trivially copyable");

        size_t currentSize = body_.size();
        body_.resize(currentSize + sizeof(TType));
        std::memcpy(body_.data() + currentSize, &data, sizeof(TType));
        return *this;
    }

    DataBuffer &operator<<(const std::string &data);

    template <typename TType>
    DataBuffer &operator>>(TType &data)
    {
        static_assert(std::is_trivially_copyable<TType>::value, "TType must be trivially copyable");

        if (read_pos_ + sizeof(TType) > body_.size())
        {
            throw BodyUnderflow();
        }
        std::memcpy(&data, body_.data() + read_pos_, sizeof(TType));
        read_pos_ += sizeof(TType);
        return *this;
    }

    DataBuffer &operator>>(std::string &data);

    virtual void clear();

    virtual std::vector<std::byte> serialize() const;
    virtual void deserialize(const std::vector<std::byte> &buffer);

    class DataBufferException : public std::runtime_error
    {
    public:
        virtual ~DataBufferException() = default;
        explicit DataBufferException(const std::string &msg);
    };

    class BodyUnderflow : public DataBufferException
    {
    public:
        virtual ~BodyUnderflow() = default;
        BodyUnderflow();
    };

    class BodySizeOutOfRange : public DataBufferException
    {
    public:
        virtual ~BodySizeOutOfRange() = default;
        BodySizeOutOfRange();
    };

    class InvalidBuffer : public DataBufferException
    {
    public:
        virtual ~InvalidBuffer() = default;
        explicit InvalidBuffer(const std::string &msg);
    };

protected:
    std::vector<std::byte> body_;
    size_t read_pos_;
};

#endif /* DATA_BUFFER_HPP */
