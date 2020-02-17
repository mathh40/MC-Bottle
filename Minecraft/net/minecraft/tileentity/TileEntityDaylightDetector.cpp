#include "TileEntityDaylightDetector.h"

void TileEntityDaylightDetector::update()
{
	if (world != nullptr && !world->isRemote && world->getTotalWorldTime() % 20L == 0L) 
	{
		blockType = getBlockType();
		if (Util::instanceof<BlockDaylightDetector>(blockType)) 
		{
			((BlockDaylightDetector*)blockType)->updatePower(world, pos);
		}
	}
}
