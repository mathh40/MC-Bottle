#pragma once
#include "ItemArrow.h"

class ItemSpectralArrow :public ItemArrow
{
public:
    EntityArrow* createArrow(World* worldIn, ItemStack stack, EntityLivingBase* shooter);
};
