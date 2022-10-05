#include "BiomeSavannaMutated.h"

BiomeSavannaMutated::BiomeSavannaMutated(BiomeProperties properties) : BiomeSavanna(properties)
{
    decorator.treesPerChunk   = 2;
    decorator.flowersPerChunk = 2;
    decorator.grassPerChunk   = 5;
}

void BiomeSavannaMutated::genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                                           double noiseVal)
{
    topBlock    = Blocks.GRASS.getDefaultState();
    fillerBlock = Blocks.DIRT.getDefaultState();
    if (noiseVal > 1.75)
    {
        topBlock    = Blocks.STONE.getDefaultState();
        fillerBlock = Blocks.STONE.getDefaultState();
    }
    else if (noiseVal > -0.5)
    {
        topBlock = Blocks.DIRT.getDefaultState().withProperty(BlockDirt.VARIANT, BlockDirt.DirtType.COARSE_DIRT);
    }

    generateBiomeTerrain(worldIn, rand, chunkPrimerIn, x, z, noiseVal);
}

void BiomeSavannaMutated::decorate(World *worldIn, pcg32 &rand, BlockPos &pos)
{
    decorator.decorate(worldIn, rand, this, pos);
}
