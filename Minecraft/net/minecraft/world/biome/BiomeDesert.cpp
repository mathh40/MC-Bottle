#include "BiomeDesert.h"

BiomeDesert::BiomeDesert(BiomeProperties properties) : Biome(properties)
{
    spawnableCreatureList.clear();
    topBlock                   = Blocks.SAND.getDefaultState();
    fillerBlock                = Blocks.SAND.getDefaultState();
    decorator.treesPerChunk    = -999;
    decorator.deadBushPerChunk = 2;
    decorator.reedsPerChunk    = 50;
    decorator.cactiPerChunk    = 10;
    spawnableCreatureList.clear();
    spawnableCreatureList.emplace_back(SpawnListEntry(EntityRabbit.class, 4, 2, 3));

    auto iterator = spawnableMonsterList.begin();

    while (true)
    {
        SpawnListEntry biome$spawnlistentry;
        do
        {
            if (!iterator.hasNext())
            {
                spawnableMonsterList.emplace_back(SpawnListEntry(EntityZombie.class, 19, 4, 4));
                spawnableMonsterList.emplace_back(SpawnListEntry(EntityZombieVillager.class, 1, 1, 1));
                spawnableMonsterList.emplace_back(SpawnListEntry(EntityHusk.class, 80, 4, 4));
                return;
            }

            biome$spawnlistentry = iterator.next();
        } while (biome$spawnlistentry.entityClass != EntityZombie.class &&
                 biome$spawnlistentry.entityClass != EntityZombieVillager.class);

        spawnableMonsterList.errase(iterator);
    }
}

void BiomeDesert::decorate(World *worldIn, pcg32 &rand, BlockPos &pos)
{
    Biome::decorate(worldIn, rand, pos);
    if (rand(1000) == 0)
    {
        auto i        = rand(16) + 8;
        auto j        = rand(16) + 8;
        auto blockpos = worldIn->getHeight(pos.add(i, 0, j)).up();
        (new WorldGenDesertWells()).generate(worldIn, rand, blockpos);
    }

    if (rand(64) == 0)
    {
        (new WorldGenFossils()).generate(worldIn, rand, pos);
    }
}
