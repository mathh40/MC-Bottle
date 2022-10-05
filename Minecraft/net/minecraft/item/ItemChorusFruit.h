#pragma once
#include "ItemFood.h"

class ItemChorusFruit : public ItemFood
{
  public:
    ItemChorusFruit(int32_t amount, float saturation);
    ItemStack onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving) override;
};
