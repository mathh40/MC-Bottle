#include "EntityMinecartEmpty.h"

#include "datafix/DataFixer.h"

EntityMinecartEmpty::EntityMinecartEmpty(World *worldIn) : EntityMinecart(worldIn)
{
}

EntityMinecartEmpty::EntityMinecartEmpty(World *worldIn, double x, double y, double z)
    : EntityMinecart(worldIn, x, y, z)
{
}

void EntityMinecartEmpty::registerFixesMinecartEmpty(DataFixer fixer)
{
    EntityMinecart::registerFixesMinecart(fixer, EntityMinecartEmpty.class);
}

bool EntityMinecartEmpty::processInitialInteract(EntityPlayer *player, EnumHand hand)
{
    if (player->isSneaking())
    {
        return false;
    }
    else if (isBeingRidden())
    {
        return true;
    }
    else
    {
        if (!world->isRemote)
        {
            player->startRiding(this);
        }

        return true;
    }
}

void EntityMinecartEmpty::onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower)
{
    if (receivingPower)
    {
        if (isBeingRidden())
        {
            removePassengers();
        }

        if (getRollingAmplitude() == 0)
        {
            setRollingDirection(-getRollingDirection());
            setRollingAmplitude(10);
            setDamage(50.0F);
            markVelocityChanged();
        }
    }
}

EntityMinecart::Type EntityMinecartEmpty::getType()
{
    return EntityMinecart::Type::RIDEABLE;
}
