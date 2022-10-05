#include "BiomeHills.h"

BiomeHills::BiomeHills(Type p_i46710_1_, BiomeProperties properties) : Biome(properties)
{
    silverfishSpawner = new WorldGenMinable(
        Blocks.MONSTER_EGG.getDefaultState().withProperty(BlockSilverfish.VARIANT, BlockSilverfish.EnumType.STONE), 9);
    spruceGenerator = new WorldGenTaiga2(false);
    if (p_i46710_1_ == Type::EXTRA_TREES)
    {
        decorator.treesPerChunk = 3;
    }

    spawnableCreatureList.emplace_back(SpawnListEntry(EntityLlama.class, 5, 4, 6));
    type = p_i46710_1_;
}

WorldGenAbstractTree BiomeHills::getRandomTreeFeature(pcg32 &rand)
{
    return (WorldGenAbstractTree)(rand(3) > 0 ? spruceGenerator : Biome::getRandomTreeFeature(rand));
}

void BiomeHills::decorate(World *worldIn, pcg32 &rand, BlockPos &pos)
{
    Biome::decorate(worldIn, rand, pos);
    auto i = 3 + rand(6);
    for (auto j1 = 0; j1 < i; ++j1)
    {
        auto k1           = rand(16);
        auto l1           = rand(28) + 4;
        auto i2           = rand(16);
        BlockPos blockpos = pos.add((int32_t)k1, l1, i2);
        if (worldIn->getBlockState(blockpos).getBlock() == Blocks.STONE)
        {
            worldIn->setBlockState(blockpos, Blocks.EMERALD_ORE.getDefaultState(), 2);
        }
    }

    for (auto j1 = 0; j1 < 7; ++j1)
    {
        auto k1 = rand(16);
        auto l1 = rand(64);
        auto i2 = rand(16);
        silverfishSpawner.generate(worldIn, rand, pos.add((int32_t)k1, l1, i2));
    }
}

void BiomeHills::genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                                  double noiseVal)
{
    topBlock    = Blocks.GRASS.getDefaultState();
    fillerBlock = Blocks.DIRT.getDefaultState();
    if ((noiseVal < -1.0 || noiseVal > 2.0) && type == Type::MUTATED)
    {
        topBlock    = Blocks.GRAVEL.getDefaultState();
        fillerBlock = Blocks.GRAVEL.getDefaultState();
    }
    else if (noiseVal > 1.0 && type != Type::EXTRA_TREES)
    {
        topBlock    = Blocks.STONE.getDefaultState();
        fillerBlock = Blocks.STONE.getDefaultState();
    }

    generateBiomeTerrain(worldIn, rand, chunkPrimerIn, x, z, noiseVal);
}
