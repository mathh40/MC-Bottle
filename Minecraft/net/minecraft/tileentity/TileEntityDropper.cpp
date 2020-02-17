#include "TileEntityDropper.h"

void TileEntityDropper::registerFixesDropper(DataFixer fixer)
{
	fixer.registerWalker(FixTypes.BLOCK_ENTITY, new ItemStackDataLists(TileEntityDropper.class, new String[]{"Items"}));
}

std::string TileEntityDropper::getName() const
{
	return hasCustomName() ? customName : "container.dropper";
}

std::string TileEntityDropper::getGuiID() const
{
	return "minecraft:dropper";
}
