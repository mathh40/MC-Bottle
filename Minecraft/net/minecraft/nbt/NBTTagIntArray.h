#pragma once
#include "NBTBase.h"

#include <vector>

class NBTTagIntArray : public NBTBase
{
  public:
    NBTTagIntArray() = default;
    NBTTagIntArray(std::vector<int32_t> p_i45132_1_);
    void write(std::ostream &var1) const override;
    void read(std::istream &var1, int32_t var2, NBTSizeTracker *var3) override;
    uint8_t getId() const override;
    std::string to_string() const override;

    friend bool operator==(const NBTTagIntArray &a, const NBTTagIntArray &b);
    std::vector<int32_t> getIntArray() const;

  private:
    std::vector<int32_t> intArray;
};
namespace std
{
template <> struct hash<NBTTagIntArray>
{
    size_t operator()(const NBTTagIntArray &x) const noexcept
    {
        auto hash = 0;

        for (auto byte : x.getIntArray())
        {
            hash = hash * 31 + std::hash<int32_t>{}(byte);
        }

        return std::hash<NBTBase>{}(x) ^ hash;
    }
};
} // namespace std
