#include "EntityEnderEye.h"

EntityEnderEye::EntityEnderEye(World *worldIn) : Entity(worldIn)
{
    setSize(0.25F, 0.25F);
}

bool EntityEnderEye::isInRangeToRenderDist(double distance)
{
    double d0 = getEntityBoundingBox().getAverageEdgeLength() * 4.0;
    if (std::isnan(d0))
    {
        d0 = 4.0;
    }

    d0 *= 64.0;
    return distance < d0 * d0;
}

void EntityEnderEye::moveTowards(const BlockPos &pos)
{
    double d0 = (double)pos.getx();
    int32_t i = pos.gety();
    double d1 = (double)pos.getz();
    double d2 = d0 - posX;
    double d3 = d1 - posZ;
    float f   = MathHelper::sqrt(d2 * d2 + d3 * d3);
    if (f > 12.0F)
    {
        targetX = posX + d2 / (double)f * 12.0;
        targetZ = posZ + d3 / (double)f * 12.0;
        targetY = posY + 8.0;
    }
    else
    {
        targetX = d0;
        targetY = (double)i;
        targetZ = d1;
    }

    despawnTimer  = 0;
    shatterOrDrop = rand(5) > 0;
}

void EntityEnderEye::setVelocity(double x, double y, double z)
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

void EntityEnderEye::onUpdate()
{
    lastTickPosX = posX;
    lastTickPosY = posY;
    lastTickPosZ = posZ;
    Entity::onUpdate();
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
    if (!world->isRemote)
    {
        double d0 = targetX - posX;
        double d1 = targetZ - posZ;
        float f1  = (float)MathHelper::sqrt(d0 * d0 + d1 * d1);
        float f2  = (float)MathHelper::atan2(d1, d0);
        double d2 = (double)f + (double)(f1 - f) * 0.0025;
        if (f1 < 1.0F)
        {
            d2 *= 0.8;
            motionY *= 0.8;
        }

        motionX = MathHelper::cos((double)f2) * d2;
        motionZ = MathHelper : sin((double)f2) * d2;
        if (posY < targetY)
        {
            motionY += (1.0 - motionY) * 0.014999999664723873;
        }
        else
        {
            motionY += (-1.0 - motionY) * 0.014999999664723873;
        }
    }

    float f3 = 0.25F;
    if (isInWater())
    {
        for (auto i = 0; i < 4; ++i)
        {
            world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX - motionX * 0.25, posY - motionY * 0.25,
                                 posZ - motionZ * 0.25, motionX, motionY, motionZ);
        }
    }
    else
    {
        world->spawnParticle(
            EnumParticleTypes::PORTAL, posX - motionX * 0.25 + MathHelper::nextDouble(rand) * 0.6 - 0.3,
            posY - motionY * 0.25 - 0.5, posZ - motionZ * 0.25 + MathHelper::nextDouble(rand) * 0.6 - 0.3, motionX,
            motionY, motionZ);
    }

    if (!world->isRemote)
    {
        setPosition(posX, posY, posZ);
        ++despawnTimer;
        if (despawnTimer > 80 && !world->isRemote)
        {
            playSound(SoundEvents::ENTITY_ENDEREYE_DEATH, 1.0F, 1.0F);
            setDead();
            if (shatterOrDrop)
            {
                world->spawnEntity(new EntityItem(world, posX, posY, posZ, ItemStack(Items::ENDER_EYE)));
            }
            else
            {
                world->playEvent(2003, BlockPos(this), 0);
            }
        }
    }
}

void EntityEnderEye::writeEntityToNBT(NBTTagCompound *compound)
{
}

void EntityEnderEye::readEntityFromNBT(NBTTagCompound *compound)
{
}

float EntityEnderEye::getBrightness()
{
    return 1.0F;
}

int32_t EntityEnderEye::getBrightnessForRender()
{
    return 15728880;
}

bool EntityEnderEye::canBeAttackedWithItem()
{
    return false;
}

EntityEnderEye::EntityEnderEye(World *worldIn, double x, double y, double z) : Entity(worldIn), despawnTimer(0)
{
    setSize(0.25F, 0.25F);
    setPosition(x, y, z);
}

void EntityEnderEye::entityInit()
{
}
