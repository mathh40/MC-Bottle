#include "EntityLightningBolt.h"

EntityLightningBolt::EntityLightningBolt(World *worldIn, double x, double y, double z, bool effectOnlyIn)
    : EntityWeatherEffect(worldIn), lightningState(2), boltVertex(MathHelper::nextLong(rand)),
      boltLivingTime(rand(3) + 1), effectOnly(effectOnlyIn)
{
    setLocationAndAngles(x, y, z, 0.0F, 0.0F);
    BlockPos blockpos(this);
    if (!effectOnlyIn && !worldIn->isRemote && worldIn->getGameRules().getBoolean("doFireTick") &&
        (worldIn->getDifficulty() == EnumDifficulty::NORMAL || worldIn->getDifficulty() == EnumDifficulty::HARD) &&
        worldIn->isAreaLoaded(blockpos, 10))
    {
        if (worldIn->getBlockState(blockpos)->getMaterial() == Material::AIR &&
            Blocks::FIRE.canPlaceBlockAt(worldIn, blockpos))
        {
            worldIn->setBlockState(blockpos, Blocks::FIRE.getDefaultState());
        }

        for (int i = 0; i < 4; ++i)
        {
            BlockPos blockpos1 = blockpos.add((int32_t)rand(3) - 1, rand(3) - 1, rand(3) - 1);
            if (worldIn->getBlockState(blockpos1)->getMaterial() == Material::AIR &&
                Blocks::FIRE.canPlaceBlockAt(worldIn, blockpos1))
            {
                worldIn->setBlockState(blockpos1, Blocks::FIRE.getDefaultState());
            }
        }
    }
}

SoundCategory EntityLightningBolt::getSoundCategory()
{
    return SoundCategory::WEATHER;
}

void EntityLightningBolt::onUpdate()
{
    EntityWeatherEffect::onUpdate();
    if (lightningState == 2)
    {
        world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_LIGHTNING_THUNDER, SoundCategory::WEATHER,
                         10000.0F, 0.8F + MathHelper::nextFloat(rand) * 0.2F);
        world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_LIGHTNING_IMPACT, SoundCategory::WEATHER, 2.0F,
                         0.5F + MathHelper::nextFloat(rand) * 0.2F);
    }

    --lightningState;
    if (lightningState < 0)
    {
        if (boltLivingTime == 0)
        {
            setDead();
        }
        else if (lightningState < -rand(10))
        {
            --boltLivingTime;
            lightningState = 1;
            if (!effectOnly && !world->isRemote)
            {
                boltVertex = MathHelper::nextLong(rand);
                BlockPos blockpos(this);
                if (world->getGameRules().getBoolean("doFireTick") && world->isAreaLoaded(blockpos, 10) &&
                    world->getBlockState(blockpos)->getMaterial() == Material::AIR &&
                    Blocks::FIRE.canPlaceBlockAt(world, blockpos))
                {
                    world->setBlockState(blockpos, Blocks::FIRE.getDefaultState());
                }
            }
        }
    }

    if (lightningState >= 0)
    {
        if (world->isRemote)
        {
            world->setLastLightningBolt(2);
        }
        else if (!effectOnly)
        {
            auto list = world->getEntitiesWithinAABBExcludingEntity(
                this, AxisAlignedBB(posX - 3.0, posY - 3.0, posZ - 3.0, posX + 3.0, posY + 6.0 + 3.0, posZ + 3.0));

            for (auto entity : list)
            {
                entity->onStruckByLightning(this);
            }
        }
    }
}

void EntityLightningBolt::entityInit()
{
}

void EntityLightningBolt::readEntityFromNBT(NBTTagCompound *compound)
{
}

void EntityLightningBolt::writeEntityToNBT(NBTTagCompound *compound)
{
}
