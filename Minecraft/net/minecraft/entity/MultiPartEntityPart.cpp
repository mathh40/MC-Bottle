#include "MultiPartEntityPart.h"

#include "DamageSource.h"
#include "IEntityMultiPart.h"

MultiPartEntityPart::MultiPartEntityPart(IEntityMultiPart *parent, std::string_view partName, float width, float height)
    : Entity(parent->getWorld()), parent(parent), partName(partName)
{
    setSize(width, height);
}

bool MultiPartEntityPart::canBeCollidedWith()
{
    return true;
}

bool MultiPartEntityPart::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    return isEntityInvulnerable(source) ? false : parent->attackEntityFromPart(this, source, amount);
}

bool MultiPartEntityPart::isEntityEqual(Entity *entityIn)
{
    return this == entityIn || parent == entityIn;
}

void MultiPartEntityPart::entityInit()
{
}

void MultiPartEntityPart::readEntityFromNBT(NBTTagCompound *compound)
{
}

void MultiPartEntityPart::writeEntityToNBT(NBTTagCompound *compound)
{
}
