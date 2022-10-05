#pragma once
#include "NBTPrimitive.h"

class NBTTagDouble : public NBTPrimitive
{
  public:
    NBTTagDouble() = default;
    explicit NBTTagDouble(double data);
    void write(std::ostream &output) const override;
    void read(std::istream &input, int depth, NBTSizeTracker *sizeTracker) override;
    uint8_t getId() const override;
    std::string to_string() const override;
    friend bool operator==(const NBTTagDouble &a, const NBTTagDouble &b);

    int64_t getLong() const override;

    int32_t getInt() const override;

    int16_t getShort() const override;

    uint8_t getByte() const override;

    double getDouble() const override;

    float getFloat() const override;

  private:
    double data{};
};

namespace std
{
template <> struct hash<NBTTagDouble>
{
    size_t operator()(const NBTTagDouble &x) const noexcept
    {
        auto i = static_cast<int64_t>(x.getDouble());
        return std::hash<NBTBase>{}(x) ^ (i ^ i >> 32);
    }
};
} // namespace std
