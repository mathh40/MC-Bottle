#pragma once
#include "Item.h"

class EntityArrow;
class ItemArrow :public Item
{
public:
    ItemArrow();
    virtual EntityArrow* createArrow(World *worldIn, ItemStack stack, EntityLivingBase* shooter);


private:
};