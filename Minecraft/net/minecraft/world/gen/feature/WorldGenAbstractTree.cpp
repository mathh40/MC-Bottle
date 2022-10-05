#include "WorldGenAbstractTree.h"

#include "Block.h"
#include "World.h"
#include "material/Material.h"
#include "state/IBlockState.h"

WorldGenAbstractTree::WorldGenAbstractTree(bool notify) : WorldGenerator(notify)
{
}

void WorldGenAbstractTree::generateSaplings(World *worldIn, pcg32 &random, const BlockPos &pos)
{
}

bool WorldGenAbstractTree::canGrowInto(Block *blockType)
{
    auto material = blockType->getDefaultState()->getMaterial();
    return material == Material::AIR || material == Material::LEAVES || blockType == Blocks::GRASS ||
           blockType == Blocks::DIRT || blockType == Blocks::LOG || blockType == Blocks::LOG2 ||
           blockType == Blocks::SAPLING || blockType == Blocks::VINE;
}

void WorldGenAbstractTree::setDirtAt(World *worldIn, const BlockPos &pos)
{
    if (worldIn->getBlockState(pos)->getBlock() != Blocks::DIRT)
    {
        setBlockAndNotifyAdequately(worldIn, pos, Blocks::DIRT.getDefaultState());
    }
}
