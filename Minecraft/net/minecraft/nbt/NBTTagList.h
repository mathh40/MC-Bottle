#pragma once
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "spdlog/spdlog.h"

class NBTTagList : public NBTBase
{
  public:
    void write(std::ostream &output) const override;
    void read(std::istream &input, int depth, NBTSizeTracker *sizeTracker) override;
    uint8_t getId() const override;
    std::string to_string() const override;
    void appendTag(std::shared_ptr<NBTBase> nbt);
    void set(int idx, std::shared_ptr<NBTBase> nbt);
    std::shared_ptr<NBTBase> removeTag(int i);
    bool isEmpty() const override;
    std::shared_ptr<NBTTagCompound> getCompoundTagAt(int i);
    int32_t getIntAt(int32_t p_186858_1_);
    std::vector<int32_t> getIntArrayAt(int32_t i);
    double getDoubleAt(int32_t i);
    float getFloatAt(int32_t i);
    std::string getStringTagAt(int32_t i);
    std::shared_ptr<NBTBase> get(int32_t idx);
    int32_t tagCount() const;
    std::vector<std::shared_ptr<NBTBase>> getTagList() const;
    friend bool operator==(const NBTTagList &a, const NBTTagList &b);
    int32_t getTagType() const;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::vector<std::shared_ptr<NBTBase>> tagList;
    mutable uint8_t tagType = 0;
};

namespace std
{
template <> struct hash<NBTTagList>
{
    size_t operator()(const NBTTagList &x) const noexcept
    {
        size_t hash = 0;

        for (const auto &byte : x.getTagList())
        {
            hash = hash * 31 + std::hash<NBTBase>{}(*byte);
        }

        return std::hash<NBTBase>{}(x) ^ hash;
    }
};
} // namespace std
