#include "ItemClock.h"

float ClockProperty::apply(ItemStack stack, World *worldIn, EntityLivingBase *entityIn)
{
    bool flag      = entityIn != nullptr;
    Entity *entity = flag ? entityIn : stack.getItemFrame();
    if (worldIn == nullptr && entity != nullptr)
    {
        worldIn = ((Entity *)entity)->world;
    }

    if (worldIn == nullptr)
    {
        return 0.0F;
    }
    else
    {
        double d0;
        if (worldIn->provider->isSurfaceWorld())
        {
            d0 = (double)worldIn->getCelestialAngle(1.0F);
        }
        else
        {
            d0 = MathHelper::random();
        }

        d0 = wobble(worldIn, d0);
        return (float)d0;
    }
}

double ClockProperty::wobble(World *p_185087_1_, double p_185087_2_)
{
    if (p_185087_1_->getTotalWorldTime() != lastUpdateTick)
    {
        lastUpdateTick = p_185087_1_->getTotalWorldTime();
        double d0      = p_185087_2_ - rotation;
        d0             = MathHelper::positiveModulo(d0 + 0.5, 1.0) - 0.5;
        rota += d0 * 0.1;
        rota *= 0.9;
        rotation = MathHelper::positiveModulo(rotation + rota, 1.0);
    }

    return rotation;
}

ItemClock::ItemClock()
{
    addPropertyOverride(ResourceLocation("time"),
                        [&](ItemStack stack, World *worldIn, EntityLivingBase *entityIn) -> float {
                            return proberty.apply(stack, worldIn, entityIn);
                        });
}
