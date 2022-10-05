#include "EntityEnderCrystal.h"

#include "../../nbt/NBTUtil.h"
#include "../../world/WorldProviderEnd.h"
#include "DamageSource.h"

DataParameter EntityEnderCrystal::BEAM_TARGET =
    EntityDataManager::createKey(EntityEnderCrystal.class, DataSerializers.OPTIONAL_BLOCK_POS);
DataParameter EntityEnderCrystal::SHOW_BOTTOM =
    EntityDataManager::createKey(EntityEnderCrystal.class, DataSerializers.BOOLEAN);

EntityEnderCrystal::EntityEnderCrystal(World *worldIn) : Entity(worldIn), innerRotation(rand(100000))
{
    preventEntitySpawning = true;
    setSize(2.0F, 2.0F);
}

EntityEnderCrystal::EntityEnderCrystal(World *worldIn, double x, double y, double z) : EntityEnderCrystal(worldIn)
{
    setPosition(x, y, z);
}

void EntityEnderCrystal::onUpdate()
{
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    ++innerRotation;
    if (!world->isRemote)
    {
        BlockPos blockpos(this);
        if (Util:: instanceof <WorldProviderEnd>(world->provider) &&
                                  world->getBlockState(blockpos)->getBlock() != Blocks::FIRE)
        {
            world->setBlockState(blockpos, Blocks::FIRE.getDefaultState());
        }
    }
}

bool EntityEnderCrystal::canBeCollidedWith()
{
    return true;
}

bool EntityEnderCrystal::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else if (Util:: instanceof <EntityDragon>(source.getTrueSource()))
    {
        return false;
    }
    else
    {
        if (!isDead && !world->isRemote)
        {
            setDead();
            if (!world->isRemote)
            {
                if (!source.isExplosion())
                {
                    world->createExplosion(nullptr, posX, posY, posZ, 6.0F, true);
                }

                onCrystalDestroyed(source);
            }
        }

        return true;
    }
}

void EntityEnderCrystal::onKillCommand()
{
    onCrystalDestroyed(DamageSource::GENERIC);
    Entity::onKillCommand();
}

void EntityEnderCrystal::setBeamTarget(std::optional<BlockPos> beamTarget)
{
    getDataManager().set(BEAM_TARGET, beamTarget);
}

std::optional<BlockPos> EntityEnderCrystal::getBeamTarget()
{
    return (getDataManager().get(BEAM_TARGET)).orNull();
}

void EntityEnderCrystal::setShowBottom(bool showBottom)
{
    getDataManager().set(SHOW_BOTTOM, showBottom);
}

bool EntityEnderCrystal::shouldShowBottom()
{
    return getDataManager().get(SHOW_BOTTOM);
}

bool EntityEnderCrystal::isInRangeToRenderDist(double distance)
{
    return Entity::isInRangeToRenderDist(distance) || getBeamTarget().has_value();
}

bool EntityEnderCrystal::canTriggerWalking()
{
    return false;
}

void EntityEnderCrystal::entityInit()
{
    getDataManager().registe(BEAM_TARGET, Optional::absent());
    getDataManager().registe(SHOW_BOTTOM, true);
}

void EntityEnderCrystal::writeEntityToNBT(NBTTagCompound *compound)
{
    if (getBeamTarget() != nullptr)
    {
        compound->setTag("BeamTarget", NBTUtil::createPosTag(getBeamTarget()));
    }

    compound->setBoolean("ShowBottom", shouldShowBottom());
}

void EntityEnderCrystal::readEntityFromNBT(NBTTagCompound *compound)
{
    if (compound->hasKey("BeamTarget", 10))
    {
        setBeamTarget(NBTUtil::getPosFromTag(compound->getCompoundTag("BeamTarget")));
    }

    if (compound->hasKey("ShowBottom", 1))
    {
        setShowBottom(compound->getBoolean("ShowBottom"));
    }
}

void EntityEnderCrystal::onCrystalDestroyed(DamageSource::DamageSource source)
{
    if (Util:: instanceof <WorldProviderEnd>(world->provider))
    {
        auto worldproviderend   = (WorldProviderEnd *)world->provider;
        auto dragonfightmanager = worldproviderend->getDragonFightManager();
        if (dragonfightmanager != nullptr)
        {
            dragonfightmanager->onCrystalDestroyed(this, source);
        }
    }
}
