#include "BlockWorldState.h"
#include "IBlockState.h"

BlockWorldState::BlockWorldState(World* worldIn, const BlockPos &posIn, bool forceLoadIn) :
    world(worldIn), pos(posIn), forceLoad(forceLoadIn) {

}

IBlockState *BlockWorldState::getBlockState() const{
    if ((state == nullptr) && (forceLoad || world->isBlockLoaded(pos))) {
        state = world->getBlockState(pos);
    }

    return state;
}

TileEntity* BlockWorldState::getTileEntity() const{
    if (tileEntity == nullptr && !tileEntityInitialized) {
        tileEntity = world->getTileEntity(pos);
        tileEntityInitialized = true;
    }

    return tileEntity;
}

BlockPos &BlockWorldState::getPos() const{
    return pos;
}
