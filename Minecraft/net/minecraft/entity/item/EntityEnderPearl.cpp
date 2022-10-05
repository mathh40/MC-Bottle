#include "EntityEnderPearl.h"

#include "../../tileentity/TileEntityEndGateway.h"
#include "EntityLivingBase.h"
#include "datafix/DataFixer.h"

EntityEnderPearl::EntityEnderPearl(World *worldIn) : EntityThrowable(worldIn)
{
}

EntityEnderPearl::EntityEnderPearl(World *worldIn, EntityLivingBase *throwerIn) : EntityThrowable(worldIn, throwerIn)
{
    perlThrower = throwerIn;
}

EntityEnderPearl::EntityEnderPearl(World *worldIn, double x, double y, double z) : EntityThrowable(worldIn, x, y, z)
{
}

void EntityEnderPearl::registerFixesEnderPearl(DataFixer fixer)
{
    EntityThrowable::registerFixesThrowable(fixer, "ThrownEnderpearl");
}

void EntityEnderPearl::onUpdate()
{
    EntityLivingBase *entitylivingbase = getThrower();
    if (entitylivingbase != nullptr && Util:: instanceof <EntityPlayer>(entitylivingbase) &&
                                                             !entitylivingbase->isEntityAlive())
    {
        setDead();
    }
    else
    {
        EntityThrowable::onUpdate();
    }
}

Entity *EntityEnderPearl::changeDimension(int32_t dimensionIn)
{
    if (thrower->dimension != dimensionIn)
    {
        thrower = nullptr;
    }

    return EntityThrowable::changeDimension(dimensionIn);
}

void EntityEnderPearl::onImpact(RayTraceResult result)
{
    EntityLivingBase *entitylivingbase = getThrower();
    if (result.entityHit != nullptr)
    {
        if (result.entityHit == perlThrower)
        {
            return;
        }

        result.entityHit->attackEntityFrom(causeThrownDamage(this, entitylivingbase), 0.0F);
    }

    if (result.typeOfHit == TraceType::BLOCK)
    {
        BlockPos blockpos      = result.getBlockPos();
        TileEntity *tileentity = world->getTileEntity(blockpos);
        if (Util:: instanceof <TileEntityEndGateway>(tileentity))
        {
            TileEntityEndGateway tileentityendgateway = (TileEntityEndGateway *)tileentity;
            if (entitylivingbase != nullptr)
            {
                if (Util:: instanceof <EntityPlayerMP>(entitylivingbase))
                {
                    CriteriaTriggers::ENTER_BLOCK::trigger((EntityPlayerMP *)entitylivingbase,
                                                           world->getBlockState(blockpos));
                }

                tileentityendgateway.teleportEntity(entitylivingbase);
                setDead();
                return;
            }

            tileentityendgateway.teleportEntity(this);
            return;
        }
    }

    for (int i = 0; i < 32; ++i)
    {
        world->spawnParticle(EnumParticleTypes::PORTAL, posX, posY + MathHelper::nextDouble(rand) * 2.0, posZ,
                             MathHelper::nextGaussian<double>(rand), 0.0, MathHelper::nextGaussian<double>(rand));
    }

    if (!world->isRemote)
    {
        if (Util:: instanceof <EntityPlayerMP>(entitylivingbase))
        {
            EntityPlayerMP *entityplayermp = (EntityPlayerMP *)entitylivingbase;
            if (entityplayermp->connection.getNetworkManager().isChannelOpen() && entityplayermp->world == world &&
                !entityplayermp->isPlayerSleeping())
            {
                if (MathHelper::nextFloat(rand) < 0.05F && world->getGameRules().getBoolean("doMobSpawning"))
                {
                    EntityEndermite *entityendermite = new EntityEndermite(world);
                    entityendermite->setSpawnedByPlayer(true);
                    entityendermite->setLocationAndAngles(entitylivingbase->posX, entitylivingbase->posY,
                                                          entitylivingbase->posZ, entitylivingbase->rotationYaw,
                                                          entitylivingbase->rotationPitch);
                    world->spawnEntity(entityendermite);
                }

                if (entitylivingbase->isRiding())
                {
                    entitylivingbase->dismountRidingEntity();
                }

                entitylivingbase->setPositionAndUpdate(posX, posY, posZ);
                entitylivingbase->fallDistance = 0.0F;
                entitylivingbase->attackEntityFrom(DamageSource::FALL, 5.0F);
            }
        }
        else if (entitylivingbase != nullptr)
        {
            entitylivingbase->setPositionAndUpdate(posX, posY, posZ);
            entitylivingbase->fallDistance = 0.0F;
        }

        setDead();
    }
}
