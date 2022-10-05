#include "DragonSpawnManager.h"

#include "biome/BiomeEndDecorator.h"

DragonSpawnManager DragonSpawnManager::START([](WorldServer *worldIn, DragonFightManager &manager,
                                                std::vector<EntityEnderCrystal *> crystals, int32_t ticks,
                                                BlockPos &pos) {
    BlockPos blockpos(0, 128, 0);

    for (auto entityendercrystal : crystals)
    {
        entityendercrystal->setBeamTarget(blockpos);
    }

    manager.setRespawnState(PREPARING_TO_SUMMON_PILLARS);
});
DragonSpawnManager DragonSpawnManager::PREPARING_TO_SUMMON_PILLARS([](WorldServer *worldIn, DragonFightManager &manager,
                                                                      std::vector<EntityEnderCrystal *> crystals,
                                                                      int32_t ticks, BlockPos &pos) {
    if (ticks < 100)
    {
        if (ticks == 0 || ticks == 50 || ticks == 51 || ticks == 52 || ticks >= 95)
        {
            BlockPos pos(0, 128, 0);
            worldIn->playEvent(3001, pos, 0);
        }
    }
    else
    {
        manager.setRespawnState(SUMMONING_PILLARS);
    }
});
DragonSpawnManager DragonSpawnManager::SUMMONING_PILLARS([](WorldServer *worldIn, DragonFightManager &manager,
                                                            std::vector<EntityEnderCrystal *> crystals, int32_t ticks,
                                                            BlockPos &pos) {
    auto flag  = ticks % 40 == 0;
    auto flag1 = ticks % 40 == 39;
    if (flag || flag1)
    {
        auto aworldgenspikes$endspike = BiomeEndDecorator::getSpikesForWorld(worldIn);
        auto j                        = ticks / 40;
        if (j < aworldgenspikes$endspike.length)
        {
            auto worldgenspikes$endspike = aworldgenspikes$endspike[j];
            if (flag)
            {
                for (auto entityendercrystal : crystals)
                {
                    entityendercrystal->setBeamTarget(new BlockPos(worldgenspikes$endspike.getCenterX(),
                                                                   worldgenspikes$endspike.getHeight() + 1,
                                                                   worldgenspikes$endspike.getCenterZ()));
                }
            }
            else
            {
                int k = true;

                BlockPos pos1(worldgenspikes$endspike.getCenterX() - 10, worldgenspikes$endspike.getHeight() - 10,
                              worldgenspikes$endspike.getCenterZ() - 10);
                BlockPos pos2(worldgenspikes$endspike.getCenterX() + 10, worldgenspikes$endspike.getHeight() + 10,
                              worldgenspikes$endspike.getCenterZ() + 10);
                for (auto blockpos$mutableblockpos : getAllInBoxMutable(pos1, pos2))
                {
                    worldIn->setBlockToAir(blockpos$mutableblockpos);
                }

                worldIn->createExplosion(nullptr, (double)((float)worldgenspikes$endspike.getCenterX() + 0.5F),
                                         (double)worldgenspikes$endspike.getHeight(),
                                         (double)((float)worldgenspikes$endspike.getCenterZ() + 0.5F), 5.0F, true);
                WorldGenSpikes worldgenspikes;
                worldgenspikes.setSpike(worldgenspikes$endspike);
                worldgenspikes.setCrystalInvulnerable(true);
                worldgenspikes.setBeamTarget(BlockPos(0, 128, 0));
                pcg32 rand;
                worldgenspikes.generate(
                    worldIn, rand,
                    BlockPos(worldgenspikes$endspike.getCenterX(), 45, worldgenspikes$endspike.getCenterZ()));
            }
        }
        else if (flag)
        {
            manager.setRespawnState(SUMMONING_DRAGON);
        }
    }
});
DragonSpawnManager DragonSpawnManager::SUMMONING_DRAGON([](WorldServer *worldIn, DragonFightManager &manager,
                                                           std::vector<EntityEnderCrystal *> crystals, int32_t ticks,
                                                           BlockPos &pos) {
    if (ticks >= 100)
    {
        manager.setRespawnState(END);
        manager.resetSpikeCrystals();

        for (auto entityendercrystal1 : crystals)
        {
            entityendercrystal1->setBeamTarget(nullptr);
            worldIn->createExplosion(entityendercrystal1, entityendercrystal1->posX, entityendercrystal1->posY,
                                     entityendercrystal1->posZ, 6.0F, false);
            entityendercrystal1->setDead();
        }
    }
    else if (ticks >= 80)
    {
        BlockPos pos1(0, 128, 0);
        worldIn->playEvent(3001, pos1, 0);
    }
    else if (ticks == 0)
    {
        for (auto entityendercrystal1 : crystals)
        {
            BlockPos pos1(0, 128, 0);
            entityendercrystal1->setBeamTarget(pos1);
        }
    }
    else if (ticks < 5)
    {
        BlockPos pos1(0, 128, 0);
        worldIn->playEvent(3001, pos1, 0);
    }
});
DragonSpawnManager DragonSpawnManager::END([](WorldServer *worldIn, DragonFightManager &manager,
                                              std::vector<EntityEnderCrystal *> crystals, int32_t ticks,
                                              BlockPos &pos) {});

void DragonSpawnManager::process(WorldServer *worldIn, DragonFightManager &manager,
                                 std::vector<EntityEnderCrystal *> crystals, int32_t ticks, BlockPos &pos)
{
    prozessfunc(worldIn, manager, crystals, ticks, pos);
}

DragonSpawnManager::DragonSpawnManager(processFunc *prozessIn)
{
    prozessfunc = prozessIn;
}
