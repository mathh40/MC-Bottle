#include "ItemMapBase.h"

#include "ItemStack.h"

bool ItemMapBase::isMap()
{
    return true;
}

Packet* ItemMapBase::createMapDataPacket(ItemStack stack, World *worldIn, EntityPlayer *player)
{
    return nullptr;
}
