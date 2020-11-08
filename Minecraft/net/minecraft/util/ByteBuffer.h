#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>

#include <vector>
#include <bit>
#include <memory>

// Default number of uint8_ts to allocate in the backing buffer if no size is provided
constexpr std::size_t BB_DEFAULT_SIZE = 4096;

template <typename T>
constexpr void SwapEndian(T &val,const std::endian& end)
{
    if (end != std::endian::native)
    {
        union U
        {
            T val;
            std::array<std::byte, sizeof(T)> raw;
        } src, dst;

        src.val = val;
        std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
        val = dst.val;
    }
}

class ByteBuffer
{
public:
    explicit ByteBuffer(std::size_t size = BB_DEFAULT_SIZE,std::endian end = std::endian::native);
    ByteBuffer(std::byte* arr, std::size_t size,std::endian end = std::endian::native);
    ~ByteBuffer() = default;

    std::size_t bytesRemaining() const; // Number of uint8_ts from the current read position till the end of the buffer
    void clear(); // Clear our the vector and reset read and write positions
    std::unique_ptr<ByteBuffer> clone() const; // Return a new instance of a ByteBuffer with the exact same contents and the same state (rpos, wpos)
    //ByteBuffer compact(); // TODO?
    bool equals(const ByteBuffer& other) const; // Compare if the contents are equivalent
    void resize(std::size_t newSize);
    std::size_t size() const; // Size of internal vector

    // Basic Searching (Linear)
    template<typename T> int32_t find(T key, std::size_t start = 0)
    {
        int32_t ret = -1;
        std::size_t len = buf.size();
        for (std::size_t i = start; i < len; i++) 
        {
            T data = read<T>(i);
            // Wasn't actually found, bounds of buffer were exceeded
            if ((key != 0) && (data == 0))
                break;

            // Key was found in array
            if (data == key) 
            {
                ret = (int32_t) i;
                break;
            }
        }
        return ret;
    }

    // Replacement
    void replace(std::byte key, std::byte rep, std::size_t start = 0, bool firstOccuranceOnly = false);

    // Read

    std::byte peek() const; // Relative peek. Reads and returns the next std::byte in the buffer from the current position but does not increment the read position
    std::byte get() const; // Relative get method. Reads the std::byte at the buffers current position then increments the position
    std::byte get(std::size_t index) const; // Absolute get method. Read std::byte at index
    void getBytes(std::byte* buf, std::size_t len) const; // Absolute read into array buf of length len
    char getChar() const; // Relative
    char getChar(std::size_t index) const; // Absolute
    double getDouble() const;
    double getDouble(std::size_t index) const;
    float getFloat() const;
    float getFloat(std::size_t index) const;
    uint32_t getInt() const;
    uint32_t getInt(std::size_t index) const;
    uint64_t getLong() const;
    uint64_t getLong(std::size_t index) const;
    uint16_t getShort() const;
    uint16_t getShort(std::size_t index) const;

    // Write

    void put(const ByteBuffer& src); // Relative write of the entire contents of another ByteBuffer (src)
    void put(std::byte b); // Relative write
    void put(std::byte b, std::size_t index); // Absolute write at index
    void putBytes(std::byte* b, std::size_t len); // Relative write
    void putBytes(std::byte* b, std::size_t len, std::size_t index); // Absolute write starting at index
    void putChar(char value); // Relative
    void putChar(char value, std::size_t index); // Absolute
    void putDouble(double value);
    void putDouble(double value, std::size_t index);
    void putFloat(float value);
    void putFloat(float value, std::size_t index);
    void putInt(uint32_t value);
    void putInt(uint32_t value, std::size_t index);
    void putLong(uint64_t value);
    void putLong(uint64_t value, std::size_t index);
    void putShort(uint16_t value);
    void putShort(uint16_t value, std::size_t index);

    // Buffer Position Accessors & Mutators

    void setReadPos(std::size_t r)
    {
        rpos = r;
    }

    std::size_t getReadPos() const
    {
        return rpos;
    }

    void setWritePos(std::size_t w)
    {
        wpos = w;
    }

    std::size_t getWritePos() const
    {
        return wpos;
    }

    // Utility Functions
#ifdef BB_UTILITY
	void setName(std::string n);
	std::string getName();
	void printInfo();
	void printAH();
	void printAscii();
	void printHex();
	void printPosition();
#endif

private:
    std::size_t wpos;
    mutable std::size_t rpos;
    std::vector<std::byte> buf;
    std::endian end;

#ifdef BB_UTILITY
	std::string name;
#endif

    template<typename T> T read() const
    {
        T data = read<T>(rpos);
        rpos += sizeof(T);
        return data;
    }

    template<typename T> T read(std::size_t index) const
    {
        if (index + sizeof(T) <= buf.size())
        {
            auto val = *((T*) &buf[index]);
            SwapEndian(val,end);
            return val;
        }
        return static_cast<T>(0);
    }

    template<typename T> void append(T data)
    {
        std::size_t s = sizeof(data);

        if (size() < (wpos + s))
        {
            buf.resize(wpos + s);
        }
        SwapEndian(data,end);
        memcpy(&buf[wpos], (std::byte*) &data, s);
        //printf("writing %c to %i\n", (uint8_t)data, wpos);

        wpos += s;
    }

    template<typename T> void insert(T data, std::size_t index)
    {
        if ((index + sizeof(data)) > size())
            return;

        SwapEndian(data,end);
        memcpy(&buf[index], (std::byte*) &data, sizeof(data));
        wpos = index + sizeof(data);
    }
};