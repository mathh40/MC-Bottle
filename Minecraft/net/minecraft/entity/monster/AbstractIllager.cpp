#include "AbstractIllager.h"

DataParameter AbstractIllager::AGGRESSIVE = EntityDataManager::createKey(AbstractIllager.class, DataSerializers.BYTE);

AbstractIllager::AbstractIllager(World *world) : EntityMob(world)
{
}

EnumCreatureAttribute AbstractIllager::getCreatureAttribute()
{
    return EnumCreatureAttribute::ILLAGER;
}

AbstractIllager::IllagerArmPose AbstractIllager::getArmPose()
{
    return AbstractIllager::IllagerArmPose::CROSSED;
}

void AbstractIllager::entityInit()
{
    EntityMob::entityInit();
    dataManager.registe(AGGRESSIVE, 0);
}

bool AbstractIllager::isAggressive(int32_t mask)
{
    int32_t i = dataManager.get(AGGRESSIVE);
    return (i & mask) != 0;
}

void AbstractIllager::setAggressive(int32_t mask, bool value)
{
    auto i = dataManager.get(AGGRESSIVE);
    if (value)
    {
        i = i | mask;
    }
    else
    {
        i = i & ~mask;
    }

    dataManager.set(AGGRESSIVE, (i & 255));
}
