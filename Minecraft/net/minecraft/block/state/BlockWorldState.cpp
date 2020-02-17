#include "BlockWorldState.h"
#include "IBlockState.h"

BlockWorldState::BlockWorldState(World& worldIn, BlockPos& posIn, bool forceLoadIn)
	:world(worldIn),pos(posIn),forceLoad(forceLoadIn)
{

}

IBlockState* BlockWorldState::getBlockState()
{
	if ((state == nullptr) && (forceLoad || world.isBlockLoaded(pos))) {
		state = world.getBlockState(pos);
	}

	return state;
}

std::optional<TileEntity> BlockWorldState::getTileEntity()
{
	if (!tileEntity.has_value() && !tileEntityInitialized) 
	{
		tileEntity = world.getTileEntity(pos);
		tileEntityInitialized = true;
	}

	return tileEntity;
}

BlockPos& BlockWorldState::getPos()
{
	return pos;
}
