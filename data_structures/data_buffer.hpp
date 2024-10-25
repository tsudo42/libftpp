#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <vector>
#include <exception>

class DataBuffer
{
public:
    DataBuffer() = default;
    virtual ~DataBuffer() = default;

    template <typename TType>
    DataBuffer &operator<<(const TType &data)
    {
        const uint8_t *rawData = reinterpret_cast<const uint8_t *>(&data);
        buffer.insert(buffer.end(), rawData, rawData + sizeof(TType));
        return *this;
    }

    class InsufficientData : public std::runtime_error
    {
    protected:
        static constexpr const char *msg = "Insufficient data in buffer.";

    public:
        InsufficientData() : std::runtime_error(msg) {}
        virtual ~InsufficientData() = default;
    };

    template <typename TType>
    DataBuffer &operator>>(TType &data)
    {
        if (buffer.size() < sizeof(TType))
        {
            throw InsufficientData();
        }

        uint8_t *rawData = reinterpret_cast<uint8_t *>(&data);
        std::copy(buffer.begin(), buffer.begin() + sizeof(TType), rawData);
        buffer.erase(buffer.begin(), buffer.begin() + sizeof(TType));
        return *this;
    }

private:
    std::vector<uint8_t> buffer;
};

#endif /* DATA_BUFFER_HPP */
