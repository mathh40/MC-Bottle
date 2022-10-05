#include "PathWorldListener.h"

#include "Path.h"
#include "SoundEvent.h"
#include "math/AxisAlignedBB.h"

void PathWorldListener::notifyBlockUpdate(World *worldIn, BlockPos &pos, IBlockState *oldState, IBlockState *newState,
                                          int32_t flags)
{
    if (didBlockChange(worldIn, pos, oldState, newState))
    {
        int i = 0;

        for (auto j = navigations.size(); i < j; ++i)
        {
            PathNavigate pathnavigate = navigations.get(i);
            if (pathnavigate != nullptr && !pathnavigate.canUpdatePathOnTimeout())
            {
                Path path = pathnavigate.getPath();
                if (path != nullptr && !path.isFinished() && path.getCurrentPathLength() != 0)
                {
                    PathPoint pathpoint = pathnavigate.currentPath.getFinalPathPoint();
                    double d0           = pos.distanceSq(((double)pathpoint.x + pathnavigate.entity->posX) / 2.0D,
                                                         ((double)pathpoint.y + pathnavigate.entity->posY) / 2.0D,
                                                         ((double)pathpoint.z + pathnavigate.entity->posZ) / 2.0D);
                    int k               = (path.getCurrentPathLength() - path.getCurrentPathIndex()) *
                            (path.getCurrentPathLength() - path.getCurrentPathIndex());
                    if (d0 < (double)k)
                    {
                        pathnavigate.updatePath();
                    }
                }
            }
        }
    }
}

void PathWorldListener::notifyLightSet(BlockPos &pos)
{
}

void PathWorldListener::markBlockRangeForRenderUpdate(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2,
                                                      int32_t z2)
{
}

void PathWorldListener::playSoundToAllNearExcept(EntityPlayer *player, SoundEvent &soundIn, SoundCategory &category,
                                                 double x, double y, double z, float volume, float pitch)
{
}

void PathWorldListener::spawnParticle(int32_t particleID, bool ignoreRange, double xCoord, double yCoord, double zCoord,
                                      double xSpeed, double ySpeed, double zSpeed,
                                      std::initializer_list<int32_t> parameters)
{
}

void PathWorldListener::onEntityAdded(Entity *entityIn)
{
    if (Util:: instanceof <EntityLiving>(entityIn))
    {
        navigations.emplace_back(((EntityLiving)entityIn)->getNavigator());
    }
}

void PathWorldListener::onEntityRemoved(Entity *entityIn)
{
    if (Util:: instanceof <EntityLiving>(entityIn))
    {
        navigations.remove(((EntityLiving)entityIn)->getNavigator());
    }
}

void PathWorldListener::playRecord(SoundEvent &soundIn, BlockPos &pos)
{
}

void PathWorldListener::broadcastSound(int32_t soundID, BlockPos &pos, int32_t data)
{
}

void PathWorldListener::playEvent(EntityPlayer *player, int32_t type, BlockPos &blockPosIn, int32_t data)
{
}

void PathWorldListener::sendBlockBreakProgress(int32_t breakerId, BlockPos &pos, int32_t progress)
{
}

bool PathWorldListener::didBlockChange(World *worldIn, BlockPos &pos, IBlockState *oldState, IBlockState *newState)
{
    AxisAlignedBB axisalignedbb  = oldState->getCollisionBoundingBox(worldIn, pos);
    AxisAlignedBB axisalignedbb1 = newState->getCollisionBoundingBox(worldIn, pos);
    return axisalignedbb != axisalignedbb1 && (axisalignedbb == nullptr || !axisalignedbb.equals(axisalignedbb1));
}
