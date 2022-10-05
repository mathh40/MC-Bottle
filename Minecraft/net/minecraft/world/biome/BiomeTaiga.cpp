#include "BiomeTaiga.h"

BiomeTaiga::BiomeTaiga(Type typeIn, BiomeProperties properties) : Biome(properties)
{
    type = typeIn;
    spawnableCreatureList.emplace_back(SpawnListEntry(EntityWolf.class, 8, 4, 4));
    spawnableCreatureList.emplace_back(SpawnListEntry(EntityRabbit.class, 4, 2, 3));
    decorator.treesPerChunk = 10;
    if (typeIn != Type::MEGA && typeIn != Type::MEGA_SPRUCE)
    {
        decorator.grassPerChunk     = 1;
        decorator.mushroomsPerChunk = 1;
    }
    else
    {
        decorator.grassPerChunk     = 7;
        decorator.deadBushPerChunk  = 1;
        decorator.mushroomsPerChunk = 3;
    }
}

WorldGenAbstractTree BiomeTaiga::getRandomTreeFeature(pcg32 &rand)
{
    if ((type == Type::MEGA || type == Type::MEGA_SPRUCE) && rand(3) == 0)
    {
        return type != Type::MEGA_SPRUCE && rand(13) != 0 ? MEGA_PINE_GENERATOR : MEGA_SPRUCE_GENERATOR;
    }
    else
    {
        return (WorldGenAbstractTree)(rand(3) == 0 ? PINE_GENERATOR : SPRUCE_GENERATOR);
    }
}

WorldGenerator BiomeTaiga::getRandomWorldGenForGrass(pcg32 &rand)
{
    return rand(5) > 0 ? new WorldGenTallGrass(BlockTallGrass.EnumType.FERN)
                       : new WorldGenTallGrass(BlockTallGrass.EnumType.GRASS);
}

void BiomeTaiga::decorate(World *worldIn, pcg32 &rand, BlockPos &pos)
{
    if (type == Type::MEGA || type == Type::MEGA_SPRUCE)
    {
        auto i1 = rand(3);

        for (auto j1 = 0; j1 < i1; ++j1)
        {
            auto k1           = rand(16) + 8;
            auto l1           = rand(16) + 8;
            BlockPos blockpos = worldIn->getHeight(pos.add(k1, 0, l1));
            FOREST_ROCK_GENERATOR.generate(worldIn, rand, blockpos);
        }
    }

    DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.FERN);

    for (auto i1 = 0; i1 < 7; ++i1)
    {
        auto j1 = rand(16) + 8;
        auto k1 = rand(16) + 8;
        auto l1 = rand(worldIn->getHeight(pos.add(j1, 0, k1)).getY() + 32);
        DOUBLE_PLANT_GENERATOR.generate(worldIn, rand, pos.add((int32_t)j1, l1, k1));
    }

    Biome::decorate(worldIn, rand, pos);
}

void BiomeTaiga::genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                                  double noiseVal)
{
    if (type == Type::MEGA || type == Type::MEGA_SPRUCE)
    {
        topBlock    = Blocks.GRASS.getDefaultState();
        fillerBlock = Blocks.DIRT.getDefaultState();
        if (noiseVal > 1.75)
        {
            topBlock = Blocks.DIRT.getDefaultState().withProperty(BlockDirt.VARIANT, BlockDirt.DirtType.COARSE_DIRT);
        }
        else if (noiseVal > -0.95)
        {
            topBlock = Blocks.DIRT.getDefaultState().withProperty(BlockDirt.VARIANT, BlockDirt.DirtType.PODZOL);
        }
    }

    generateBiomeTerrain(worldIn, rand, chunkPrimerIn, x, z, noiseVal);
}
