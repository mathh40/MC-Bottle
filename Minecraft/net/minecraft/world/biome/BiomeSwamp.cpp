#include "BiomeSwamp.h"

IBlockState *BiomeSwamp::WATER_LILY = Blocks.WATERLILY.getDefaultState();

BiomeSwamp::BiomeSwamp(BiomeProperties properties) : Biome(properties)
{
    decorator.treesPerChunk         = 2;
    decorator.flowersPerChunk       = 1;
    decorator.deadBushPerChunk      = 1;
    decorator.mushroomsPerChunk     = 8;
    decorator.reedsPerChunk         = 10;
    decorator.clayPerChunk          = 1;
    decorator.waterlilyPerChunk     = 4;
    decorator.sandPatchesPerChunk   = 0;
    decorator.gravelPatchesPerChunk = 0;
    decorator.grassPerChunk         = 5;
    spawnableMonsterList.emplace_back(SpawnListEntry(EntitySlime.class, 1, 1, 1));
}

WorldGenAbstractTree BiomeSwamp::getRandomTreeFeature(pcg32 &rand)
{
    return SWAMP_FEATURE;
}

int32_t BiomeSwamp::getGrassColorAtPos(BlockPos &pos)
{
    double d0 = GRASS_COLOR_NOISE.getValue((double)pos.getx() * 0.0225, (double)pos.getz() * 0.0225);
    return d0 < -0.1 ? 5011004 : 6975545;
}

int32_t BiomeSwamp::getFoliageColorAtPos(BlockPos &pos)
{
    return 6975545;
}

BlockFlower.EnumFlowerType BiomeSwamp::pickRandomFlower(pcg32 &rand, BlockPos &pos)
{
    return BlockFlower.EnumFlowerType.BLUE_ORCHID;
}

void BiomeSwamp::genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                                  double noiseVal)
{
    double d0 = GRASS_COLOR_NOISE.getValue((double)x * 0.25, (double)z * 0.25);
    if (d0 > 0.0)
    {
        auto i = x & 15;
        auto j = z & 15;

        for (auto k = 255; k >= 0; --k)
        {
            if (chunkPrimerIn.getBlockState(j, k, i).getMaterial() != Material::AIR)
            {
                if (k == 62 && chunkPrimerIn.getBlockState(j, k, i).getBlock() != Blocks.WATER)
                {
                    chunkPrimerIn.setBlockState(j, k, i, WATER);
                    if (d0 < 0.12)
                    {
                        chunkPrimerIn.setBlockState(j, k + 1, i, WATER_LILY);
                    }
                }
                break;
            }
        }
    }

    generateBiomeTerrain(worldIn, rand, chunkPrimerIn, x, z, noiseVal);
}

void BiomeSwamp::decorate(World *worldIn, pcg32 &rand, BlockPos &pos)
{
    Biome::decorate(worldIn, rand, pos);
    if (rand(64) == 0)
    {
        (new WorldGenFossils()).generate(worldIn, rand, pos);
    }
}
