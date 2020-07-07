#include "EntityAIFollowOwnerFlying.h"

#include "state/IBlockState.h"

EntityAIFollowOwnerFlying::EntityAIFollowOwnerFlying(EntityTameable *tameableIn, double followSpeedIn, float minDistIn,
                                                     float maxDistIn)
        :EntityAIFollowOwner(tameableIn, followSpeedIn, minDistIn, maxDistIn)
{
}

bool EntityAIFollowOwnerFlying::isTeleportFriendlyBlock(int32_t x, int32_t z, int32_t y, int32_t xOffset,
    int32_t zOffset)
{
    IBlockState* iblockstate = world->getBlockState(BlockPos(x + xOffset, y - 1, z + zOffset));
    return (iblockstate->isTopSolid() || iblockstate->getMaterial() == Material::LEAVES) && world->isAirBlock(BlockPos(x + xOffset, y, z + zOffset)) && world->isAirBlock(BlockPos(x + xOffset, y + 1, z + zOffset));
}
