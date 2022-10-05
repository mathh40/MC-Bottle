#pragma once
#include "ItemFood.h"

class ItemSoup : public ItemFood
{
  public:
    ItemSoup(int32_t healAmount);
    ItemStack onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving) override;
};
