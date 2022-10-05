#pragma once
#include "NBTBase.h"

#include <vector>

class NBTTagLongArray : public NBTBase
{
  public:
    NBTTagLongArray() = default;
    NBTTagLongArray(std::vector<int64_t> p_i47524_1_);
    void write(std::ostream &var1) const override;
    void read(std::istream &var1, int var2, NBTSizeTracker var3) override;
    uint8_t getId() const override;
    std::string to_string() const override;

    friend bool operator==(const NBTTagLongArray &a, const NBTTagLongArray &b);
    std::vector<int64_t> getLongArray() const;

  private:
    std::vector<int64_t> data;
};

namespace std
{
template <> struct hash<NBTTagLongArray>
{
    size_t operator()(const NBTTagLongArray &x) const noexcept
    {
        auto hash = 0;

        for (auto byte : x.getLongArray())
        {
            hash = hash * 31 + std::hash<int32_t>{}(byte);
        }

        return std::hash<NBTBase>{}(x) ^ hash;
    }
};
} // namespace std
