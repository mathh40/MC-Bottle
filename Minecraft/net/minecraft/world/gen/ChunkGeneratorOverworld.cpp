#include "ChunkGeneratorOverworld.h"

#include "ChunkGeneratorSettings.h"
#include "WorldEntitySpawner.h"
#include "chunk/Chunk.h"
#include "chunk/ChunkPrimer.h"
#include "math/MathHelper.h"

IBlockState *ChunkGeneratorOverworld::STONE = Blocks::STONE.getDefaultState();

ChunkGeneratorOverworld::ChunkGeneratorOverworld(World *worldIn, int64_t seed, bool mapFeaturesEnabledIn,
                                                 std::string_view generatorOptions)
    : rand(seed), world(worldIn), mapFeaturesEnabled(mapFeaturesEnabledIn)
{
    oceanBlock                = Blocks::WATER.getDefaultState();
    caveGenerator             = new MapGenCaves();
    strongholdGenerator       = new MapGenStronghold();
    villageGenerator          = new MapGenVillage();
    mineshaftGenerator        = new MapGenMineshaft();
    scatteredFeatureGenerator = new MapGenScatteredFeature();
    ravineGenerator           = new MapGenRavine();
    oceanMonumentGenerator    = new StructureOceanMonument();
    woodlandMansionGenerator  = new WoodlandMansion(this);
    terrainType               = worldIn->getWorldInfo().getTerrainType();
    minLimitPerlinNoise       = new NoiseGeneratorOctaves(rand, 16);
    maxLimitPerlinNoise       = new NoiseGeneratorOctaves(rand, 16);
    mainPerlinNoise           = new NoiseGeneratorOctaves(rand, 8);
    surfaceNoise              = new NoiseGeneratorPerlin(rand, 4);
    scaleNoise                = new NoiseGeneratorOctaves(rand, 10);
    depthNoise                = new NoiseGeneratorOctaves(rand, 16);
    forestNoise               = new NoiseGeneratorOctaves(rand, 8);

    for (auto i = -2; i <= 2; ++i)
    {
        for (auto j = -2; j <= 2; ++j)
        {
            auto f                            = 10.0F / MathHelper::sqrt((float)(i * i + j * j) + 0.2F);
            biomeWeights[i + 2 + (j + 2) * 5] = f;
        }
    }

    settings   = ChunkGeneratorSettings::Factory::jsonToFactory(generatorOptions).build();
    oceanBlock = settings.useLavaOceans ? Blocks::LAVA.getDefaultState() : Blocks::WATER.getDefaultState();
    worldIn->setSeaLevel(settings.seaLevel);
}

void ChunkGeneratorOverworld::setBlocksInChunk(int32_t x, int32_t z, ChunkPrimer &primer)
{
    biomesForGeneration =
        world->getBiomeProvider().getBiomesForGeneration(biomesForGeneration, x * 4 - 2, z * 4 - 2, 10, 10);
    generateHeightmap(x * 4, 0, z * 4);

    for (int i = 0; i < 4; ++i)
    {
        auto j = i * 5;
        auto k = (i + 1) * 5;

        for (auto l = 0; l < 4; ++l)
        {
            auto i1 = (j + l) * 33;
            auto j1 = (j + l + 1) * 33;
            auto k1 = (k + l) * 33;
            auto l1 = (k + l + 1) * 33;

            for (auto i2 = 0; i2 < 32; ++i2)
            {
                auto d1 = heightMap[i1 + i2];
                auto d2 = heightMap[j1 + i2];
                auto d3 = heightMap[k1 + i2];
                auto d4 = heightMap[l1 + i2];
                auto d5 = (heightMap[i1 + i2 + 1] - d1) * 0.125;
                auto d6 = (heightMap[j1 + i2 + 1] - d2) * 0.125;
                auto d7 = (heightMap[k1 + i2 + 1] - d3) * 0.125;
                auto d8 = (heightMap[l1 + i2 + 1] - d4) * 0.125;

                for (auto j2 = 0; j2 < 8; ++j2)
                {
                    auto d10 = d1;
                    auto d11 = d2;
                    auto d12 = (d3 - d1) * 0.25;
                    auto d13 = (d4 - d2) * 0.25;

                    for (auto k2 = 0; k2 < 4; ++k2)
                    {
                        auto d16       = (d11 - d10) * 0.25;
                        auto lvt_45_1_ = d10 - d16;

                        for (auto l2 = 0; l2 < 4; ++l2)
                        {
                            if ((lvt_45_1_ += d16) > 0.0)
                            {
                                primer.setBlockState(i * 4 + k2, i2 * 8 + j2, l * 4 + l2, STONE);
                            }
                            else if (i2 * 8 + j2 < settings.seaLevel)
                            {
                                primer.setBlockState(i * 4 + k2, i2 * 8 + j2, l * 4 + l2, oceanBlock);
                            }
                        }

                        d10 += d12;
                        d11 += d13;
                    }

                    d1 += d5;
                    d2 += d6;
                    d3 += d7;
                    d4 += d8;
                }
            }
        }
    }
}

void ChunkGeneratorOverworld::replaceBiomeBlocks(int32_t x, int32_t z, ChunkPrimer &primer,
                                                 std::vector<Biome *> biomesIn)
{
    double d0   = 0.03125;
    depthBuffer = surfaceNoise.getRegion(depthBuffer, (double)(x * 16), (double)(z * 16), 16, 16, 0.0625, 0.0625, 1.0);

    for (auto i = 0; i < 16; ++i)
    {
        for (auto j = 0; j < 16; ++j)
        {
            auto biome = biomesIn[j + i * 16];
            biome->genTerrainBlocks(world, rand, primer, x * 16 + i, z * 16 + j, depthBuffer[j + i * 16]);
        }
    }
}

Chunk ChunkGeneratorOverworld::generateChunk(int32_t x, int32_t z)
{
    rand.seed((long)x * 341873128712L + (long)z * 132897987541L);
    ChunkPrimer chunkprimer;
    setBlocksInChunk(x, z, chunkprimer);
    biomesForGeneration = world->getBiomeProvider().getBiomes(biomesForGeneration, x * 16, z * 16, 16, 16);
    replaceBiomeBlocks(x, z, chunkprimer, biomesForGeneration);
    if (settings.useCaves)
    {
        caveGenerator.generate(world, x, z, chunkprimer);
    }

    if (settings.useRavines)
    {
        ravineGenerator.generate(world, x, z, chunkprimer);
    }

    if (mapFeaturesEnabled)
    {
        if (settings.useMineShafts)
        {
            mineshaftGenerator.generate(world, x, z, chunkprimer);
        }

        if (settings.useVillages)
        {
            villageGenerator.generate(world, x, z, chunkprimer);
        }

        if (settings.useStrongholds)
        {
            strongholdGenerator.generate(world, x, z, chunkprimer);
        }

        if (settings.useTemples)
        {
            scatteredFeatureGenerator.generate(world, x, z, chunkprimer);
        }

        if (settings.useMonuments)
        {
            oceanMonumentGenerator.generate(world, x, z, chunkprimer);
        }

        if (settings.useMansions)
        {
            woodlandMansionGenerator.generate(world, x, z, chunkprimer);
        }
    }

    Chunk chunk(world, chunkprimer, x, z);
    auto abyte = chunk.getBiomeArray();

    for (auto i = 0; i < abyte.size(); ++i)
    {
        abyte[i] = Biome::getIdForBiome(biomesForGeneration[i]);
    }

    chunk.generateSkylightMap();
    return chunk;
}

std::vector<SpawnListEntry> ChunkGeneratorOverworld::getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos)
{
    auto biome = world->getBiome(pos);
    if (mapFeaturesEnabled)
    {
        if (creatureType == EnumCreatureType::MONSTER && scatteredFeatureGenerator.isSwampHut(pos))
        {
            return scatteredFeatureGenerator.getMonsters();
        }

        if (creatureType == EnumCreatureType::MONSTER && settings.useMonuments &&
            oceanMonumentGenerator.isPositionInStructure(world, pos))
        {
            return oceanMonumentGenerator.getMonsters();
        }
    }

    return biome.getSpawnableList(creatureType);
}

bool ChunkGeneratorOverworld::isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos)
{
    if (!mapFeaturesEnabled)
    {
        return false;
    }
    else if ("Stronghold" == (structureName) && strongholdGenerator != nullptr)
    {
        return strongholdGenerator.isInsideStructure(pos);
    }
    else if ("Mansion" == (structureName) && woodlandMansionGenerator != nullptr)
    {
        return woodlandMansionGenerator.isInsideStructure(pos);
    }
    else if ("Monument" == (structureName) && oceanMonumentGenerator != nullptr)
    {
        return oceanMonumentGenerator.isInsideStructure(pos);
    }
    else if ("Village" == (structureName) && villageGenerator != nullptr)
    {
        return villageGenerator.isInsideStructure(pos);
    }
    else if ("Mineshaft" == (structureName) && mineshaftGenerator != nullptr)
    {
        return mineshaftGenerator.isInsideStructure(pos);
    }
    else
    {
        return "Temple" == (structureName) && scatteredFeatureGenerator != nullptr
                   ? scatteredFeatureGenerator.isInsideStructure(pos)
                   : false;
    }
}

std::optional<BlockPos> ChunkGeneratorOverworld::getNearestStructurePos(World *worldIn, std::string_view structureName,
                                                                        BlockPos &position, bool findUnexplored)
{
    if (!mapFeaturesEnabled)
    {
        return std::nullopt;
    }
    else if ("Stronghold" == (structureName) && strongholdGenerator != nullptr)
    {
        return strongholdGenerator.getNearestStructurePos(worldIn, position, findUnexplored);
    }
    else if ("Mansion" == (structureName) && woodlandMansionGenerator != nullptr)
    {
        return woodlandMansionGenerator.getNearestStructurePos(worldIn, position, findUnexplored);
    }
    else if ("Monument" == (structureName) && oceanMonumentGenerator != nullptr)
    {
        return oceanMonumentGenerator.getNearestStructurePos(worldIn, position, findUnexplored);
    }
    else if ("Village" == (structureName) && villageGenerator != nullptr)
    {
        return villageGenerator.getNearestStructurePos(worldIn, position, findUnexplored);
    }
    else if ("Mineshaft" == (structureName) && mineshaftGenerator != nullptr)
    {
        return mineshaftGenerator.getNearestStructurePos(worldIn, position, findUnexplored);
    }
    else
    {
        return "Temple" == (structureName) && scatteredFeatureGenerator != nullptr
                   ? scatteredFeatureGenerator.getNearestStructurePos(worldIn, position, findUnexplored)
                   : std::nullopt;
    }
}

void ChunkGeneratorOverworld::recreateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    if (mapFeaturesEnabled)
    {
        if (settings.useMineShafts)
        {
            mineshaftGenerator.generate(world, x, z, std::nullopt);
        }

        if (settings.useVillages)
        {
            villageGenerator.generate(world, x, z, std::nullopt);
        }

        if (settings.useStrongholds)
        {
            strongholdGenerator.generate(world, x, z, std::nullopt);
        }

        if (settings.useTemples)
        {
            scatteredFeatureGenerator.generate(world, x, z, std::nullopt);
        }

        if (settings.useMonuments)
        {
            oceanMonumentGenerator.generate(world, x, z, std::nullopt);
        }

        if (settings.useMansions)
        {
            woodlandMansionGenerator.generate(world, x, z, std::nullopt);
        }
    }
}

void ChunkGeneratorOverworld::populate(int32_t x, int32_t z)
{
    BlockFalling::fallInstantly = true;
    auto i                      = x * 16;
    auto j                      = z * 16;
    BlockPos blockpos(i, 0, j);
    auto biome = world->getBiome(blockpos.add(16, 0, 16));
    rand.seed(world->getSeed());
    std::uniform_int_distribution<int64_t> dist;
    auto k = dist(rand) / 2L * 2L + 1L;
    auto l = dist(rand) / 2L * 2L + 1L;
    rand.seed((long)x * k + (long)z * l ^ world->getSeed());
    bool flag = false;
    ChunkPos chunkpos(x, z);
    if (mapFeaturesEnabled)
    {
        if (settings.useMineShafts)
        {
            mineshaftGenerator.generateStructure(world, rand, chunkpos);
        }

        if (settings.useVillages)
        {
            flag = villageGenerator.generateStructure(world, rand, chunkpos);
        }

        if (settings.useStrongholds)
        {
            strongholdGenerator.generateStructure(world, rand, chunkpos);
        }

        if (settings.useTemples)
        {
            scatteredFeatureGenerator.generateStructure(world, rand, chunkpos);
        }

        if (settings.useMonuments)
        {
            oceanMonumentGenerator.generateStructure(world, rand, chunkpos);
        }

        if (settings.useMansions)
        {
            woodlandMansionGenerator.generateStructure(world, rand, chunkpos);
        }
    }

    if (biome != Biomes::DESERT && biome != Biomes::DESERT_HILLS && settings.useWaterLakes && !flag &&
        rand(settings.waterLakeChance) == 0)
    {
        auto k2 = rand(16) + 8;
        auto j3 = rand(256);
        auto l3 = rand(16) + 8;
        (new WorldGenLakes(Blocks.WATER)).generate(world, rand, blockpos.add(k2, j3, l3));
    }

    if (!flag && rand(settings.lavaLakeChance / 10) == 0 && settings.useLavaLakes)
    {
        auto k2 = rand(16) + 8;
        auto j3 = rand(rand(248) + 8);
        auto l3 = rand(16) + 8;
        if (j3 < world->getSeaLevel() || rand(settings.lavaLakeChance / 8) == 0)
        {
            (new WorldGenLakes(Blocks.LAVA)).generate(world, rand, blockpos.add(k2, j3, l3));
        }
    }

    if (settings.useDungeons)
    {
        for (auto k2 = 0; k2 < settings.dungeonChance; ++k2)
        {
            auto j3 = rand(16) + 8;
            auto l3 = rand(256);
            int l1  = rand(16) + 8;
            (new WorldGenDungeons()).generate(world, rand, blockpos.add(j3, l3, l1));
        }
    }

    biome.decorate(world, rand, BlockPos(i, 0, j));
    WorldEntitySpawner.performWorldGenSpawning(world, biome, i + 8, j + 8, 16, 16, rand);
    blockpos = blockpos.add(8, 0, 8);

    for (auto k2 = 0; k2 < 16; ++k2)
    {
        for (auto j3 = 0; j3 < 16; ++j3)
        {
            BlockPos blockpos1 = world->getPrecipitationHeight(blockpos.add(k2, 0, j3));
            BlockPos blockpos2 = blockpos1.down();
            if (world->canBlockFreezeWater(blockpos2))
            {
                world->setBlockState(blockpos2, Blocks::ICE.getDefaultState(), 2);
            }

            if (world->canSnowAt(blockpos1, true))
            {
                world->setBlockState(blockpos1, Blocks::SNOW_LAYER.getDefaultState(), 2);
            }
        }
    }

    BlockFalling::fallInstantly = false;
}

bool ChunkGeneratorOverworld::generateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    bool flag = false;
    if (settings.useMonuments && mapFeaturesEnabled && chunkIn.getInhabitedTime() < 3600L)
    {
        flag |= oceanMonumentGenerator.generateStructure(world, rand, ChunkPos(x, z));
    }

    return flag;
}

void ChunkGeneratorOverworld::generateHeightmap(int32_t x, int32_t y, int32_t z)
{
    depthRegion =
        depthNoise.generateNoiseOctaves(depthRegion, x, z, 5, 5, (double)settings.depthNoiseScaleX,
                                        (double)settings.depthNoiseScaleZ, (double)settings.depthNoiseScaleExponent);
    auto f          = settings.coordinateScale;
    auto f1         = settings.heightScale;
    mainNoiseRegion = mainPerlinNoise.generateNoiseOctaves(
        mainNoiseRegion, x, y, z, 5, 33, 5, (double)(f / settings.mainNoiseScaleX),
        (double)(f1 / settings.mainNoiseScaleY), (double)(f / settings.mainNoiseScaleZ));
    minLimitRegion =
        minLimitPerlinNoise.generateNoiseOctaves(minLimitRegion, x, y, z, 5, 33, 5, (double)f, (double)f1, (double)f);
    maxLimitRegion =
        maxLimitPerlinNoise.generateNoiseOctaves(maxLimitRegion, x, y, z, 5, 33, 5, (double)f, (double)f1, (double)f);
    auto i = 0;
    auto j = 0;

    for (auto k = 0; k < 5; ++k)
    {
        for (auto l = 0; l < 5; ++l)
        {
            auto f2    = 0.0F;
            auto f3    = 0.0F;
            auto f4    = 0.0F;
            auto biome = biomesForGeneration[k + 2 + (l + 2) * 10];

            for (auto j1 = -2; j1 <= 2; ++j1)
            {
                for (auto k1 = -2; k1 <= 2; ++k1)
                {
                    auto biome1 = biomesForGeneration[k + j1 + 2 + (l + k1 + 2) * 10];
                    auto f5     = settings.biomeDepthOffSet + biome1->getBaseHeight() * settings.biomeDepthWeight;
                    auto f6     = settings.biomeScaleOffset + biome1->getHeightVariation() * settings.biomeScaleWeight;
                    if (terrainType == WorldType::AMPLIFIED && f5 > 0.0F)
                    {
                        f5 = 1.0F + f5 * 2.0F;
                        f6 = 1.0F + f6 * 4.0F;
                    }

                    auto f7 = biomeWeights[j1 + 2 + (k1 + 2) * 5] / (f5 + 2.0F);
                    if (biome1->getBaseHeight() > biome->getBaseHeight())
                    {
                        f7 /= 2.0F;
                    }

                    f2 += f6 * f7;
                    f3 += f5 * f7;
                    f4 += f7;
                }
            }

            f2 /= f4;
            f3 /= f4;
            f2      = f2 * 0.9F + 0.1F;
            f3      = (f3 * 4.0F - 1.0F) / 8.0F;
            auto d7 = depthRegion[j] / 8000.0;
            if (d7 < 0.0)
            {
                d7 = -d7 * 0.3;
            }

            d7 = d7 * 3.0 - 2.0;
            if (d7 < 0.0)
            {
                d7 /= 2.0;
                if (d7 < -1.0)
                {
                    d7 = -1.0;
                }

                d7 /= 1.4;
                d7 /= 2.0;
            }
            else
            {
                if (d7 > 1.0)
                {
                    d7 = 1.0;
                }

                d7 /= 8.0;
            }

            ++j;
            auto d8 = (double)f3;
            auto d9 = (double)f2;
            d8 += d7 * 0.2;
            d8      = d8 * (double)settings.baseSize / 8.0;
            auto d0 = (double)settings.baseSize + d8 * 4.0;

            for (auto l1 = 0; l1 < 33; ++l1)
            {
                auto d1 = ((double)l1 - d0) * (double)settings.stretchY * 128.0 / 256.0 / d9;
                if (d1 < 0.0)
                {
                    d1 *= 4.0;
                }

                auto d2 = minLimitRegion[i] / (double)settings.lowerLimitScale;
                auto d3 = maxLimitRegion[i] / (double)settings.upperLimitScale;
                auto d4 = (mainNoiseRegion[i] / 10.0 + 1.0) / 2.0;
                auto d5 = MathHelper::clampedLerp(d2, d3, d4) - d1;
                if (l1 > 29)
                {
                    auto d6 = (double)((float)(l1 - 29) / 3.0F);
                    d5      = d5 * (1.0 - d6) + -10.0 * d6;
                }

                heightMap[i] = d5;
                ++i;
            }
        }
    }
}
