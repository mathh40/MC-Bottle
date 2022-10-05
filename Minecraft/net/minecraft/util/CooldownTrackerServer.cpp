#include "CooldownTrackerServer.h"

#include "../entity/player/EntityPlayerMP.h"

CooldownTrackerServer::CooldownTrackerServer(EntityPlayerMP *playerIn) : player(playerIn)
{
}

void CooldownTrackerServer::notifyOnSet(Item *itemIn, uint32_t ticksIn)
{
    CooldownTracker::notifyOnSet(itemIn, ticksIn);
    player->connection.sendPacket(new SPacketCooldown(itemIn, ticksIn));
}

void CooldownTrackerServer::notifyOnRemove(Item *itemIn)
{
    CooldownTracker::notifyOnRemove(itemIn);
    player->connection.sendPacket(new SPacketCooldown(itemIn, 0));
}
