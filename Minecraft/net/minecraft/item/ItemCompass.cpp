#include "ItemCompass.h"

#include "ItemStack.h"
#include "../world/biome/Biome.h"

float CompassProperty::apply(ItemStack stack, World* worldIn, EntityLivingBase* entityIn)
{
    if (entityIn == nullptr && !stack.isOnItemFrame()) 
    {
        return 0.0F;
    }
    else 
    {
        bool flag = entityIn != nullptr;
        Entity* entity = flag ? entityIn : stack.getItemFrame();
        if (worldIn == nullptr) 
        {
            worldIn = ((Entity*)entity)->world;
        }

        double d0;
        if (worldIn->provider.isSurfaceWorld()) 
        {
            double d1 = flag ? (double)((Entity*)entity)->rotationYaw : getFrameRotation((EntityItemFrame*)entity);
            d1 = MathHelper::positiveModulo(d1 / 360.0, 1.0);
            double d2 = getSpawnToAngle(worldIn, (Entity*)entity) / 6.283185307179586;
            d0 = 0.5 - (d1 - 0.25 - d2);
        }
        else 
        {
            d0 = MathHelper::random();
        }

        if (flag) 
        {
            d0 = wobble(worldIn, d0);
        }

        return MathHelper::positiveModulo((float)d0, 1.0F);
    }
}

double CompassProperty::wobble(World* worldIn, double p_185093_2_)
{
    if (worldIn->getTotalWorldTime() != lastUpdateTick) 
    {
        lastUpdateTick = worldIn->getTotalWorldTime();
        double d0 = p_185093_2_ - rotation;
        d0 = MathHelper::positiveModulo(d0 + 0.5, 1.0) - 0.5;
        rota += d0 * 0.1;
        rota *= 0.8;
        rotation = MathHelper::positiveModulo(rotation + rota, 1.0);
    }

    return rotation;
}

double CompassProperty::getFrameRotation(EntityItemFrame* p_185094_1_)
{
    return (double)MathHelper::wrapDegrees(180 + p_185094_1_->facingDirection.getHorizontalIndex() * 90);
}

double CompassProperty::getSpawnToAngle(World* p_185092_1_, Entity* p_185092_2_)
{
    BlockPos blockpos = p_185092_1_.getSpawnPoint();
    return MathHelper::atan2((double)blockpos.getz() - p_185092_2_->posZ, (double)blockpos.getx() - p_185092_2_->posX);
}


ItemCompass::ItemCompass()
{
    addPropertyOverride(ResourceLocation("angle"),[&](ItemStack stack, World *worldIn, EntityLivingBase *entityIn)->float
    {
        property.apply(stack,worldIn,entityIn);
    });
}
