#pragma once
#include "Item.h"

class EntityArrow;
class ItemArrow :public Item
{
public:
    ItemArrow();
    EntityArrow* createArrow(World *worldIn, ItemStack stack, EntityLivingBase* shooter);
private:
};