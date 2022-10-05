#include "EntityThrowable.h"

#include "../EntityLivingBase.h"
#include "WorldServer.h"
#include "datafix/DataFixer.h"

EntityThrowable::EntityThrowable(World *worldIn) : Entity(worldIn), xTile(-1), yTile(-1), zTile(-1)
{
    setSize(0.25F, 0.25F);
}

EntityThrowable::EntityThrowable(World *worldIn, double x, double y, double z) : EntityThrowable(worldIn)
{
    setPosition(x, y, z);
}

EntityThrowable::EntityThrowable(World *worldIn, EntityLivingBase *throwerIn)
    : EntityThrowable(worldIn, throwerIn->posX,
                      throwerIn->posY + (double)throwerIn->getEyeHeight() - 0.10000000149011612, throwerIn->posZ)
{
}

bool EntityThrowable::isInRangeToRenderDist(double distance)
{
    double d0 = getEntityBoundingBox().getAverageEdgeLength() * 4.0;
    if (std::isnan(d0))
    {
        d0 = 4.0;
    }

    d0 *= 64.0;
    return distance < d0 * d0;
}

void EntityThrowable::shoot(Entity *entityThrower, float rotationPitchIn, float rotationYawIn, float pitchOffset,
                            float velocity, float inaccuracy)
{
    float f  = -MathHelper::sin(rotationYawIn * 0.017453292F) * MathHelper::cos(rotationPitchIn * 0.017453292F);
    float f1 = -MathHelper::sin((rotationPitchIn + pitchOffset) * 0.017453292F);
    float f2 = MathHelper::cos(rotationYawIn * 0.017453292F) * MathHelper::cos(rotationPitchIn * 0.017453292F);
    shoot((double)f, (double)f1, (double)f2, velocity, inaccuracy);
    motionX += entityThrower->motionX;
    motionZ += entityThrower->motionZ;
    if (!entityThrower->onGround)
    {
        motionY += entityThrower->motionY;
    }
}

void EntityThrowable::shoot(double x, double y, double z, float velocity, float inaccuracy)
{
    float f = MathHelper::sqrt(x * x + y * y + z * z);
    x /= (double)f;
    y /= (double)f;
    z /= (double)f;
    x += MathHelper::nextGaussian(rand) * 0.007499999832361937 * (double)inaccuracy;
    y += MathHelper::nextGaussian(rand) * 0.007499999832361937 * (double)inaccuracy;
    z += MathHelper::nextGaussian(rand) * 0.007499999832361937 * (double)inaccuracy;
    x *= (double)velocity;
    y *= (double)velocity;
    z *= (double)velocity;
    motionX           = x;
    motionY           = y;
    motionZ           = z;
    float f1          = MathHelper::sqrt(x * x + z * z);
    rotationYaw       = (float)(MathHelper::atan2(x, z) * 57.29577951308232);
    rotationPitch     = (float)(MathHelper::atan2(y, (double)f1) * 57.29577951308232);
    prevRotationYaw   = rotationYaw;
    prevRotationPitch = rotationPitch;
    ticksInGround     = 0;
}

void EntityThrowable::setVelocity(double x, double y, double z)
{
    motionX = x;
    motionY = y;
    motionZ = z;
    if (prevRotationPitch == 0.0F && prevRotationYaw == 0.0F)
    {
        float f           = MathHelper::sqrt(x * x + z * z);
        rotationYaw       = (float)(MathHelper::atan2(x, z) * 57.29577951308232);
        rotationPitch     = (float)(MathHelper::atan2(y, (double)f) * 57.29577951308232);
        prevRotationYaw   = rotationYaw;
        prevRotationPitch = rotationPitch;
    }
}

void EntityThrowable::onUpdate()
{
    lastTickPosX = posX;
    lastTickPosY = posY;
    lastTickPosZ = posZ;
    Entity::onUpdate();
    if (throwableShake > 0)
    {
        --throwableShake;
    }

    if (inGround)
    {
        if (world->getBlockState(BlockPos(xTile, yTile, zTile))->getBlock() == inTile)
        {
            ++ticksInGround;
            if (ticksInGround == 1200)
            {
                setDead();
            }

            return;
        }

        inGround = false;
        motionX *= (double)(MathHelper::nextFloat(rand) * 0.2F);
        motionY *= (double)(MathHelper::nextFloat(rand) * 0.2F);
        motionZ *= (double)(MathHelper::nextFloat(rand) * 0.2F);
        ticksInGround = 0;
        ticksInAir    = 0;
    }
    else
    {
        ++ticksInAir;
    }

    Vec3d vec3d         = Vec3d(posX, posY, posZ);
    Vec3d vec3d1        = Vec3d(posX + motionX, posY + motionY, posZ + motionZ);
    auto raytraceresult = world->rayTraceBlocks(vec3d, vec3d1);
    vec3d               = Vec3d(posX, posY, posZ);
    vec3d1              = Vec3d(posX + motionX, posY + motionY, posZ + motionZ);
    if (raytraceresult.has_value())
    {
        vec3d1 = Vec3d(raytraceresult->hitVec.getx(), raytraceresult->hitVec.gety(), raytraceresult->hitVec.getz());
    }

    Entity *entity = nullptr;
    auto list      = world->getEntitiesWithinAABBExcludingEntity(
             this, getEntityBoundingBox().expand(motionX, motionY, motionZ).grow(1.0));
    double d0 = 0.0;
    bool flag = false;

    for (auto entity1 : list)
    {
        if (entity1->canBeCollidedWith())
        {
            if (entity1 == ignoreEntity)
            {
                flag = true;
            }
            else if (thrower != nullptr && ticksExisted < 2 && ignoreEntity == nullptr)
            {
                ignoreEntity = entity1;
                flag         = true;
            }
            else
            {
                flag                        = false;
                AxisAlignedBB axisalignedbb = entity1->getEntityBoundingBox().grow(0.30000001192092896);
                auto raytraceresult1        = axisalignedbb.calculateIntercept(vec3d, vec3d1);
                if (raytraceresult1.has_value())
                {
                    double d1 = vec3d.squareDistanceTo(raytraceresult1->hitVec);
                    if (d1 < d0 || d0 == 0.0)
                    {
                        entity = entity1;
                        d0     = d1;
                    }
                }
            }
        }
    }

    if (ignoreEntity != nullptr)
    {
        if (flag)
        {
            ignoreTime = 2;
        }
        else if (ignoreTime-- <= 0)
        {
            ignoreEntity = nullptr;
        }
    }

    if (entity != nullptr)
    {
        raytraceresult = RayTraceResult(entity);
    }

    if (raytraceresult.has_value())
    {
        if (raytraceresult->typeOfHit == TraceType::BLOCK &&
            world->getBlockState(raytraceresult->getBlockPos())->getBlock() == Blocks::PORTAL)
        {
            setPortal(raytraceresult->getBlockPos());
        }
        else
        {
            onImpact(raytraceresult);
        }
    }

    posX += motionX;
    posY += motionY;
    posZ += motionZ;
    float f     = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
    rotationYaw = (float)(MathHelper::atan2(motionX, motionZ) * 57.29577951308232);

    for (rotationPitch = (float)(MathHelper::atan2(motionY, (double)f) * 57.29577951308232);
         rotationPitch - prevRotationPitch < -180.0F; prevRotationPitch -= 360.0F)
    {
    }

    while (rotationPitch - prevRotationPitch >= 180.0F)
    {
        prevRotationPitch += 360.0F;
    }

    while (rotationYaw - prevRotationYaw < -180.0F)
    {
        prevRotationYaw -= 360.0F;
    }

    while (rotationYaw - prevRotationYaw >= 180.0F)
    {
        prevRotationYaw += 360.0F;
    }

    rotationPitch = prevRotationPitch + (rotationPitch - prevRotationPitch) * 0.2F;
    rotationYaw   = prevRotationYaw + (rotationYaw - prevRotationYaw) * 0.2F;
    float f1      = 0.99F;
    float f2      = getGravityVelocity();
    if (isInWater())
    {
        for (int j = 0; j < 4; ++j)
        {
            float f3 = 0.25F;
            world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX - motionX * 0.25, posY - motionY * 0.25,
                                 posZ - motionZ * 0.25, motionX, motionY, motionZ);
        }

        f1 = 0.8F;
    }

    motionX *= (double)f1;
    motionY *= (double)f1;
    motionZ *= (double)f1;
    if (!hasNoGravity())
    {
        motionY -= (double)f2;
    }

    setPosition(posX, posY, posZ);
}

void EntityThrowable::registerFixesThrowable(DataFixer fixer, std::string_view name)
{
}

void EntityThrowable::writeEntityToNBT(NBTTagCompound *compound)
{
    compound->setInteger("xTile", xTile);
    compound->setInteger("yTile", yTile);
    compound->setInteger("zTile", zTile);
    ResourceLocation resourcelocation = (ResourceLocation)Block::REGISTRY.getNameForObject(inTile);
    compound->setString("inTile", resourcelocation == nullptr ? "" : resourcelocation.to_string());
    compound->setByte("shake", throwableShake);
    compound->setByte("inGround", (inGround ? 1 : 0));
    if ((throwerName.empty()) && Util:: instanceof <EntityPlayer>(thrower))
    {
        throwerName = thrower->getName();
    }

    compound->setString("ownerName", throwerName);
}

void EntityThrowable::readEntityFromNBT(NBTTagCompound *compound)
{
    xTile = compound->getInteger("xTile");
    yTile = compound->getInteger("yTile");
    zTile = compound->getInteger("zTile");
    if (compound->hasKey("inTile", 8))
    {
        inTile = Block::getBlockFromName(compound->getString("inTile"));
    }
    else
    {
        inTile = Block::getBlockById(compound->getByte("inTile") & 255);
    }

    throwableShake = compound->getByte("shake") & 255;
    inGround       = compound->getByte("inGround") == 1;
    thrower        = nullptr;
    throwerName    = compound->getString("ownerName");

    thrower = getThrower();
}

EntityLivingBase *EntityThrowable::getThrower()
{
    if (thrower == nullptr && !throwerName.empty())
    {
        thrower = world->getPlayerEntityByName(throwerName);
        if (thrower == nullptr && Util:: instanceof <WorldServer>(world))
        {
            try
            {
                Entity *entity = ((WorldServer *)world)->getEntityFromUuid(xg::Guid(throwerName));
                if (Util:: instanceof <EntityLivingBase>(entity))
                {
                    thrower = (EntityLivingBase *)entity;
                }
            }
            catch (std::exception &var2)
            {
                thrower = nullptr;
            }
        }
    }

    return thrower;
}

void EntityThrowable::entityInit()
{
}

float EntityThrowable::getGravityVelocity()
{
    return 0.03F;
}
