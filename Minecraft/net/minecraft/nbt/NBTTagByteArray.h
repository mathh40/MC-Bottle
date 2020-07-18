#pragma once
#include <vector>
#include "NBTPrimitive.h"


class NBTTagByteArray : public NBTBase
{
public:
    NBTTagByteArray() = default;
    explicit NBTTagByteArray(const ByteBuffer &data);
    void write(std::ostream &output) const override;
    void read(std::istream &input, int depth, NBTSizeTracker *sizeTracker) override;
    uint8_t getId() const override;
    std::string to_string() const override;
    friend bool operator==(const NBTTagByteArray &a, const NBTTagByteArray &b);
    const ByteBuffer getByteArray() const;

private:
    ByteBuffer data{};
};

namespace std
{
    template <>
    struct hash<NBTTagByteArray>
    {
        size_t operator()(const NBTTagByteArray &x) const noexcept
        {
            auto hash = 0;

            for (auto byte : x.getByteArray())
            {
                hash = hash * 31 + std::hash<uint8_t>{}(byte);
            }

            return std::hash<NBTBase>{}(x) ^ hash;
        }
    };
} // namespace std
