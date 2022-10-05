#include "BlockFalling.h"

#include <random>

BlockFalling::BlockFalling() : Block(Material.SAND)
{
    setCreativeTab(CreativeTabs.BUILDING_BLOCKS);
}

BlockFalling::BlockFalling(Material materialIn) : Block(materialIn)
{
}

void BlockFalling::onBlockAdded(World worldIn, BlockPos pos, IBlockState state)
{
    worldIn.scheduleUpdate(pos, this, tickRate(worldIn));
}

void BlockFalling::neighborChanged(IBlockState state, World worldIn, BlockPos pos, Block blockIn, BlockPos fromPos)
{
    worldIn.scheduleUpdate(pos, this, tickRate(worldIn));
}

void BlockFalling::updateTick(World worldIn, BlockPos pos, IBlockState state, pcg32 &rand)
{
    if (!worldIn.isRemote)
    {
        checkFallable(worldIn, pos);
    }
}

void BlockFalling::checkFallable(World worldIn, BlockPos pos)
{
    if (canFallThrough(worldIn.getBlockState(pos.down())) && pos.gety() >= 0)
    {
        int i = true;
        if (!fallInstantly && worldIn.isAreaLoaded(pos.add(-32, -32, -32), pos.add(32, 32, 32)))
        {
            if (!worldIn.isRemote)
            {
                auto entityfallingblock = EntityFallingBlock(worldIn, (double)pos.getx() + 0.5, (double)pos.gety(),
                                                             (double)pos.getz() + 0.5, worldIn.getBlockState(pos));
                onStartFalling(entityfallingblock);
                worldIn.spawnEntity(entityfallingblock);
            }
        }
        else
        {
            worldIn.setBlockToAir(pos);

            BlockPos blockpos;
            for (blockpos = pos.down(); canFallThrough(worldIn.getBlockState(blockpos)) && blockpos.gety() > 0;
                 blockpos = blockpos.down())
            {
            }

            if (blockpos.gety() > 0)
            {
                worldIn.setBlockState(blockpos.up(), getDefaultState());
            }
        }
    }
}

void BlockFalling::onStartFalling(EntityFallingBlock fallingEntity)
{
}

int32_t tickRate(World worldIn)
{
    return 2;
}

bool BlockFalling::canFallThrough(IBlockState state)
{
    auto block    = state.getBlock();
    auto material = state.getMaterial();
    return block == Blocks.FIRE || material == Material.AIR || material == Material.WATER || material == Material.LAVA;
}

void BlockFalling::onEndFalling(World worldIn, BlockPos pos, IBlockState fallingState, IBlockState hitState)
{
}

void BlockFalling::onBroken(World worldIn, BlockPos pos)
{
}

void BlockFalling::randomDisplayTick(IBlockState stateIn, World worldIn, BlockPos pos, pcg32 &rand)
{
    if (rand(16) == 0)
    {
        auto blockpos = pos.down();
        if (canFallThrough(worldIn.getBlockState(blockpos)))
        {
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);
            double d0 = (double)((float)pos.getx() + dist(rand));
            double d1 = (double)pos.gety() - 0.05;
            double d2 = (double)((float)pos.getz() + dist(rand));
            worldIn.spawnParticle(EnumParticleTypes::FALLING_DUST, d0, d1, d2, 0.0, 0.0, 0.0,
                                  Block.getStateId(stateIn));
        }
    }
}

int32_t BlockFalling::getDustColor(IBlockState state)
{
    return -16777216;
}
