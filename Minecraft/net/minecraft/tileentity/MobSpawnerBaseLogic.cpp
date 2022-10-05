#include "MobSpawnerBaseLogic.h"

#include "../entity/EntityLiving.h"
#include "../world/World.h"
#include "EnumParticleTypes.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"

void MobSpawnerBaseLogic::setEntityId(std::optional<ResourceLocation> id)
{
    if (id)
    {
        spawnData.getNbt()->setString("id", id.value().to_string());
    }
}

void MobSpawnerBaseLogic::updateSpawner()
{
    if (!isActivated())
    {
        prevMobRotation = mobRotation;
    }
    else
    {
        BlockPos blockpos = getSpawnerPosition();
        if (getSpawnerWorld()->isRemote)
        {
            double d3 = (double)((float)blockpos.getx() + MathHelper::nextFloat(getSpawnerWorld()->rand));
            double d4 = (double)((float)blockpos.gety() + MathHelper::nextFloat(getSpawnerWorld()->rand));
            double d5 = (double)((float)blockpos.getz() + MathHelper::nextFloat(getSpawnerWorld()->rand));
            getSpawnerWorld()->spawnParticle(EnumParticleTypes::SMOKE_NORMAL, d3, d4, d5, 0.0, 0.0, 0.0);
            getSpawnerWorld()->spawnParticle(EnumParticleTypes::FLAME, d3, d4, d5, 0.0, 0.0, 0.0);
            if (spawnDelay > 0)
            {
                --spawnDelay;
            }

            prevMobRotation = mobRotation;
            MathHelper::fmod(mobRotation + (double)(1000.0F / ((float)spawnDelay + 200.0F)), 360.0);
        }
        else
        {
            if (spawnDelay == -1)
            {
                resetTimer();
            }

            if (spawnDelay > 0)
            {
                --spawnDelay;
                return;
            }

            bool flag = false;
            int32_t i = 0;

            while (true)
            {
                if (i >= spawnCount)
                {
                    if (flag)
                    {
                        resetTimer();
                    }
                    break;
                }

                auto nbttagcompound = spawnData.getNbt();
                auto nbttaglist     = nbttagcompound->getTagList("Pos", 6);
                auto world          = getSpawnerWorld();
                int32_t j           = nbttaglist->tagCount();
                double d0           = j >= 1 ? nbttaglist->getDoubleAt(0)
                                             : (double)blockpos.getx() +
                                         (MathHelper::nextDouble(world->rand) - MathHelper::nextDouble(world->rand)) *
                                             (double)spawnRange +
                                         0.5;
                double d1   = j >= 2 ? nbttaglist->getDoubleAt(1) : (double)(blockpos.gety() + world->rand(3) - 1);
                double d2   = j >= 3 ? nbttaglist->getDoubleAt(2)
                                     : (double)blockpos.getz() +
                                         (MathHelper::nextDouble(world->rand) - MathHelper::nextDouble(world->rand)) *
                                             (double)spawnRange +
                                         0.5;
                auto entity = AnvilChunkLoader::readWorldEntityPos(nbttagcompound, world, d0, d1, d2, false);
                if (entity == nullptr)
                {
                    return;
                }

                int k = world
                            ->getEntitiesWithinAABB(
                                entity->getClass(),
                                (AxisAlignedBB((double)blockpos.getx(), (double)blockpos.gety(),
                                               (double)blockpos.getz(), (double)(blockpos.getx() + 1),
                                               (double)(blockpos.gety() + 1), (double)(blockpos.getz() + 1)))
                                    .grow((double)spawnRange))
                            .size();
                if (k >= maxNearbyEntities)
                {
                    resetTimer();
                    return;
                }

                auto entityliving = Util:: instanceof <EntityLiving>(entity) ? (EntityLiving *)entity : nullptr;
                entity->setLocationAndAngles(entity->posX, entity->posY, entity->posZ,
                                             MathHelper::nextFloat(world->rand) * 360.0F, 0.0F);
                if (entityliving == nullptr || entityliving.getCanSpawnHere() && entityliving.isNotColliding())
                {
                    if (spawnData.getNbt()->getSize() == 1 && spawnData.getNbt()->hasKey("id", 8) && Util:: instanceof
                        <EntityLiving>(entity))
                    {
                        ((EntityLiving *)entity)
                            ->onInitialSpawn(world->getDifficultyForLocation(BlockPos(entity)), nullptr);
                    }

                    AnvilChunkLoader::spawnEntity(entity, world);
                    world->playEvent(2004, blockpos, 0);
                    if (entityliving != nullptr)
                    {
                        entityliving.spawnExplosionParticle();
                    }

                    flag = true;
                }

                ++i;
            }
        }
    }
}

void MobSpawnerBaseLogic::readFromNBT(NBTTagCompound *nbt)
{
    spawnDelay = nbt->getShort("Delay");
    potentialSpawns.clear();
    if (nbt->hasKey("SpawnPotentials", 9))
    {
        auto nbttaglist = nbt->getTagList("SpawnPotentials", 10);

        for (auto i = 0; i < nbttaglist->tagCount(); ++i)
        {
            potentialSpawns.emplace_back(WeightedSpawnerEntity(nbttaglist->getCompoundTagAt(i)));
        }
    }

    if (nbt->hasKey("SpawnData", 10))
    {
        setNextSpawnData(WeightedSpawnerEntity(1, nbt->getCompoundTag("SpawnData")));
    }
    else if (!potentialSpawns.empty())
    {
        setNextSpawnData(
            (WeightedSpawnerEntity)WeightedRandom::getRandomItem(getSpawnerWorld()->rand, potentialSpawns));
    }

    if (nbt->hasKey("MinSpawnDelay", 99))
    {
        minSpawnDelay = nbt->getShort("MinSpawnDelay");
        maxSpawnDelay = nbt->getShort("MaxSpawnDelay");
        spawnCount    = nbt->getShort("SpawnCount");
    }

    if (nbt->hasKey("MaxNearbyEntities", 99))
    {
        maxNearbyEntities         = nbt->getShort("MaxNearbyEntities");
        activatingRangeFromPlayer = nbt->getShort("RequiredPlayerRange");
    }

    if (nbt->hasKey("SpawnRange", 99))
    {
        spawnRange = nbt->getShort("SpawnRange");
    }

    if (getSpawnerWorld() != nullptr)
    {
        cachedEntity = nullptr;
    }
}

NBTTagCompound *MobSpawnerBaseLogic::writeToNBT(NBTTagCompound *p_189530_1_)
{
    auto resourcelocation = getEntityId();
    if (!resourcelocation.has_value())
    {
        return p_189530_1_;
    }
    else
    {
        p_189530_1_->setShort("Delay", (short)spawnDelay);
        p_189530_1_->setShort("MinSpawnDelay", (short)minSpawnDelay);
        p_189530_1_->setShort("MaxSpawnDelay", (short)maxSpawnDelay);
        p_189530_1_->setShort("SpawnCount", (short)spawnCount);
        p_189530_1_->setShort("MaxNearbyEntities", (short)maxNearbyEntities);
        p_189530_1_->setShort("RequiredPlayerRange", (short)activatingRangeFromPlayer);
        p_189530_1_->setShort("SpawnRange", (short)spawnRange);
        p_189530_1_->setTag("SpawnData", spawnData.getNbt().copy());
        NBTTagList *nbttaglist = new NBTTagList();
        if (potentialSpawns.empty())
        {
            nbttaglist->appendTag(spawnData.toCompoundTag());
        }
        else
        {
            for (auto weightedspawnerentity : potentialSpawns)
            {
                nbttaglist->appendTag(weightedspawnerentity.toCompoundTag());
            }
        }

        p_189530_1_->setTag("SpawnPotentials", nbttaglist);
        return p_189530_1_;
    }
}

Entity *MobSpawnerBaseLogic::getCachedEntity()
{
    if (cachedEntity == nullptr)
    {
        cachedEntity = AnvilChunkLoader::readWorldEntity(spawnData.getNbt(), getSpawnerWorld(), false);
        if (spawnData.getNbt().getSize() == 1 && spawnData.getNbt().hasKey("id", 8) && Util:: instanceof
            <EntityLiving>(cachedEntity))
        {
            ((EntityLiving *)cachedEntity)
                ->onInitialSpawn(getSpawnerWorld()->getDifficultyForLocation(BlockPos(cachedEntity)), nullptr);
        }
    }

    return cachedEntity;
}

bool MobSpawnerBaseLogic::setDelayToMin(int32_t delay)
{
    if (delay == 1 && getSpawnerWorld()->isRemote)
    {
        spawnDelay = minSpawnDelay;
        return true;
    }
    else
    {
        return false;
    }
}

void MobSpawnerBaseLogic::setNextSpawnData(WeightedSpawnerEntity p_184993_1_)
{
    spawnData = p_184993_1_;
}

double MobSpawnerBaseLogic::getMobRotation() const
{
    return mobRotation;
}

double MobSpawnerBaseLogic::getPrevMobRotation() const
{
    return prevMobRotation;
}

std::optional<ResourceLocation> MobSpawnerBaseLogic::getEntityId()
{
    auto s = spawnData.getNbt()->getString("id");
    return StringUtils.isNullOrEmpty(s) ? nullptr : new ResourceLocation(s);
}

bool MobSpawnerBaseLogic::isActivated()
{
    BlockPos blockpos = getSpawnerPosition();
    return getSpawnerWorld()->isAnyPlayerWithinRangeAt((double)blockpos.getx() + 0.5, (double)blockpos.gety() + 0.5,
                                                       (double)blockpos.getz() + 0.5,
                                                       (double)activatingRangeFromPlayer);
}

void MobSpawnerBaseLogic::resetTimer()
{
    if (maxSpawnDelay <= minSpawnDelay)
    {
        spawnDelay = minSpawnDelay;
    }
    else
    {
        int32_t i  = maxSpawnDelay - minSpawnDelay;
        spawnDelay = minSpawnDelay + getSpawnerWorld()->rand(i);
    }

    if (!potentialSpawns.empty())
    {
        setNextSpawnData(
            (WeightedSpawnerEntity)WeightedRandom::getRandomItem(getSpawnerWorld()->rand, potentialSpawns));
    }

    broadcastEvent(std::byte{1});
}
