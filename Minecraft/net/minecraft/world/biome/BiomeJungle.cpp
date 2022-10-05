#include "BiomeJungle.h"

IBlockState *BiomeJungle::JUNGLE_LOG =
    Blocks.LOG.getDefaultState().withProperty(BlockOldLog.VARIANT, BlockPlanks.EnumType.JUNGLE);
IBlockState *BiomeJungle::JUNGLE_LEAF = Blocks.LEAVES.getDefaultState()
                                            .withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.JUNGLE)
                                            .withProperty(BlockLeaves.CHECK_DECAY, false);
IBlockState *BiomeJungle::OAK_LEAF = Blocks.LEAVES.getDefaultState()
                                         .withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.OAK)
                                         .withProperty(BlockLeaves.CHECK_DECAY, false);

BiomeJungle::BiomeJungle(bool isEdgeIn, BiomeProperties properties) : Biome(properties)
{
    isEdge = isEdgeIn;
    if (isEdgeIn)
    {
        decorator.treesPerChunk = 2;
    }
    else
    {
        decorator.treesPerChunk = 50;
    }

    decorator.grassPerChunk   = 25;
    decorator.flowersPerChunk = 4;
    if (!isEdgeIn)
    {
        spawnableMonsterList.emplace_back(SpawnListEntry(EntityOcelot.class, 2, 1, 1));
    }

    spawnableCreatureList.emplace_back(SpawnListEntry(EntityParrot.class, 40, 1, 2));
    spawnableCreatureList.emplace_back(SpawnListEntry(EntityChicken.class, 10, 4, 4));
}

WorldGenAbstractTree BiomeJungle::getRandomTreeFeature(pcg32 &rand)
{
    if (rand(10) == 0)
    {
        return BIG_TREE_FEATURE;
    }
    else if (rand(2) == 0)
    {
        return new WorldGenShrub(JUNGLE_LOG, OAK_LEAF);
    }
    else
    {
        return (WorldGenAbstractTree)(!isEdge && rand(3) == 0
                                          ? new WorldGenMegaJungle(false, 10, 20, JUNGLE_LOG, JUNGLE_LEAF)
                                          : new WorldGenTrees(false, 4 + rand(7), JUNGLE_LOG, JUNGLE_LEAF, true));
    }
}

WorldGenerator BiomeJungle::getRandomWorldGenForGrass(pcg32 &rand)
{
    return rand(4) == 0 ? new WorldGenTallGrass(BlockTallGrass.EnumType.FERN)
                        : new WorldGenTallGrass(BlockTallGrass.EnumType.GRASS);
}

void BiomeJungle::decorate(World *worldIn, pcg32 &rand, BlockPos &pos)
{
    Biome::decorate(worldIn, rand, pos);
    auto i = rand(16) + 8;
    auto j = rand(16) + 8;
    auto k = rand(worldIn->getHeight(pos.add(i, 0, j)).gety() * 2);
    (new WorldGenMelon()).generate(worldIn, rand, pos.add((int32_t)i, k, j));
    WorldGenVines worldgenvines = new WorldGenVines();

    for (auto j1 = 0; j1 < 50; ++j1)
    {
        k       = rand(16) + 8;
        auto i1 = rand(16) + 8;
        worldgenvines.generate(worldIn, rand, pos.add(k, 128, i1));
    }
}
