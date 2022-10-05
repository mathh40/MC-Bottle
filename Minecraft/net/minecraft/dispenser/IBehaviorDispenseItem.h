#pragma once
#include "../item/ItemStack.h"

class IBlockSource;

class IBehaviorDispenseItem
{
  protected:
    virtual ~IBehaviorDispenseItem() = default;

  private:
    virtual ItemStack dispense(IBlockSource *var1, const ItemStack &var2) = 0;
};

class DefaultBehaviorDispenseItem : public IBehaviorDispenseItem
{
  public:
    ItemStack dispense(IBlockSource *source, const ItemStack &stack) override;
};
