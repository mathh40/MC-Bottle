#pragma once
#include "IBlockBehaviors.h"
#include "IBlockProperties.h"

#include <functional>
#include <unordered_map>

class IProperty;
class Block;

class IBlockState : public IBlockBehaviors, public IBlockProperties
{
  protected:
    ~IBlockState() override = default;

  public:
    virtual std::vector<const std::string> getPropertyKeys()                       = 0;
    virtual std::function<bool()> getValue(IProperty *var1)                        = 0;
    virtual IBlockState *withProperty(IProperty *var1, std::function<bool()> var2) = 0;
    virtual IBlockState *cycleProperty(IProperty *var1)                            = 0;
    virtual std::unordered_map<uint64_t, IProperty *> getProperties()              = 0;
    virtual Block *getBlock()                                                      = 0;
};
