#pragma once
#include <ostream>

class NBTSizeTracker;

class NBTBase
{
  public:
    static std::string NBT_TYPES[];

    virtual ~NBTBase()                                                        = default;
    virtual void write(std::ostream &var1) const                              = 0;
    virtual void read(std::istream &var1, int32_t var2, NBTSizeTracker *var3) = 0;
    virtual std::string to_string() const                                     = 0;
    virtual uint8_t getId() const                                             = 0;

    static std::shared_ptr<NBTBase> create(uint8_t id);
    static std::string_view getTypeName(int id);

    virtual bool isEmpty() const;
    virtual std::string getString() const;

  protected:
    NBTBase() = default;
};

bool operator==(const NBTBase &a, const NBTBase &b);

namespace std
{
template <> struct hash<NBTBase>
{
    size_t operator()(const NBTBase &x) const noexcept
    {
        return x.getId();
    }
};
} // namespace std
