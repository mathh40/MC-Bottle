#pragma once
#include "Item.h"

class EntityItemFrame;
class CompassProperty
{
public:
    float apply(ItemStack stack, World *worldIn, EntityLivingBase *entityIn);
private:
    double rotation;
    double rota;
    long lastUpdateTick;
    double wobble(World *worldIn, double p_185093_2_);
    double getFrameRotation(EntityItemFrame* p_185094_1_);
    double getSpawnToAngle(World *p_185092_1_, Entity *p_185092_2_);
};

class ItemCompass :public Item
{
public:
    ItemCompass();
private:
    CompassProperty property;
};