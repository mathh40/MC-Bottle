#include "ServerWorldEventHandler.h"

#include "../util/Util.h"
#include "../util/math/BlockPos.h"
ServerWorldEventHandler::ServerWorldEventHandler(MinecraftServer &mcServerIn, WorldServer &worldServerIn)
    : server(mcServerIn), world(worldServerIn)
{
}

void ServerWorldEventHandler::notifyBlockUpdate(World *worldIn, BlockPos &pos, IBlockState *oldState,
                                                IBlockState *newState, int32_t flags)
{
    world.getPlayerChunkMap().markBlockForUpdate(pos);
}

void ServerWorldEventHandler::notifyLightSet(BlockPos &pos)
{
}

void ServerWorldEventHandler::markBlockRangeForRenderUpdate(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2,
                                                            int32_t z2)
{
}

void ServerWorldEventHandler::playSoundToAllNearExcept(EntityPlayer *player, SoundEvent &soundIn,
                                                       SoundCategory &category, double x, double y, double z,
                                                       float volume, float pitch)
{
    server.getPlayerList().sendToAllNearExcept(player, x, y, z, volume > 1.0F ? (16.0F * volume) : 16.0,
                                               world.provider.getDimensionType().getId(),
                                               SPacketSoundEffect(soundIn, category, x, y, z, volume, pitch));
}

void ServerWorldEventHandler::playRecord(SoundEvent &soundIn, BlockPos &pos)
{
}

void ServerWorldEventHandler::spawnParticle(int32_t particleID, bool ignoreRange, double xCoord, double yCoord,
                                            double zCoord, double xSpeed, double ySpeed, double zSpeed,
                                            std::initializer_list<int32_t> parameters)
{
}

void ServerWorldEventHandler::spawnParticle(int32_t id, bool ignoreRange, bool minimiseParticleLevel, double x,
                                            double y, double z, double xSpeed, double ySpeed, double zSpeed,
                                            std::initializer_list<int32_t> parameters)
{
}

void ServerWorldEventHandler::onEntityAdded(Entity *entityIn)
{
    world.getEntityTracker().track(entityIn);
    if (Util:: instanceof <EntityPlayerMP>(entityIn))
    {
        world.provider.onPlayerAdded((EntityPlayerMP)entityIn);
    }
}

void ServerWorldEventHandler::onEntityRemoved(Entity *entityIn)
{
    world.getEntityTracker().untrack(entityIn);
    world.getScoreboard().removeEntity(entityIn);
    if (Util:: instanceof <EntityPlayerMP>(entityIn))
    {
        world.provider.onPlayerRemoved((EntityPlayerMP)entityIn);
    }
}

void ServerWorldEventHandler::broadcastSound(int32_t soundID, BlockPos &pos, int32_t data)
{
    server.getPlayerList().sendPacketToAllPlayers(SPacketEffect(soundID, pos, data, true));
}

void ServerWorldEventHandler::playEvent(EntityPlayer *player, int32_t type, BlockPos &blockPosIn, int32_t data)
{
    server.getPlayerList().sendToAllNearExcept(player, blockPosIn.getx(), blockPosIn.gety(), blockPosIn.getz(), 64.0,
                                               world.provider.getDimensionType().getId(),
                                               SPacketEffect(type, blockPosIn, data, false));
}

void ServerWorldEventHandler::sendBlockBreakProgress(int32_t breakerId, BlockPos &pos, int32_t progress)
{
    for (auto entityplayermp : server.getPlayerList().getPlayers())
    {
        if (entityplayermp != nullptr && entityplayermp.world == world && entityplayermp.getEntityId() != breakerId)
        {
            double d0 = (double)pos.getx() - entityplayermp.posX;
            double d1 = (double)pos.gety() - entityplayermp.posY;
            double d2 = (double)pos.getz() - entityplayermp.posZ;
            if (d0 * d0 + d1 * d1 + d2 * d2 < 1024.0)
            {
                entityplayermp.connection.sendPacket(SPacketBlockBreakAnim(breakerId, pos, progress));
            }
        }
    }
}
