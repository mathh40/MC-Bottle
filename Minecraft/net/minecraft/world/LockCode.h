#pragma once
#include <string>

class NBTTagCompound;

class LockCode
{
  public:
    static LockCode EMPTY_CODE;
    LockCode(std::string code = "");
    bool isEmpty() const;
    std::string getLock() const;
    void toNBT(NBTTagCompound *nbt) const;
    static LockCode fromNBT(NBTTagCompound *nbt);

  private:
    std::string lock;
};
