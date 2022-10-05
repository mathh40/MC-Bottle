#include "ChunkGeneratorEnd.h"

#include "chunk/Chunk.h"
#include "chunk/ChunkPrimer.h"

IBlockState *ChunkGeneratorEnd::END_STONE = Blocks::END_STONE.getDefaultState();
IBlockState *ChunkGeneratorEnd::AIR       = Blocks::AIR.getDefaultState();

ChunkGeneratorEnd::ChunkGeneratorEnd(World *p_i47241_1_, bool p_i47241_2_, int64_t p_i47241_3_, BlockPos &p_i47241_5_)
    : world(p_i47241_1_), mapFeaturesEnabled(p_i47241_2_), spawnPoint(p_i47241_5_), rand(p_i47241_3_),
      lperlinNoise1(rand, 16), lperlinNoise2(rand, 16), perlinNoise1(rand, 8), noiseGen5(rand, 10), noiseGen6(rand, 16),
      islandNoise(rand)
{
}

void ChunkGeneratorEnd::setBlocksInChunk(int32_t x, int32_t z, ChunkPrimer &primer)
{
    buffer = getHeights(buffer, x * 2, 0, z * 2, 3, 33, 3);

    for (auto i1 = 0; i1 < 2; ++i1)
    {
        for (auto j1 = 0; j1 < 2; ++j1)
        {
            for (auto k1 = 0; k1 < 32; ++k1)
            {
                auto d0 = 0.25;
                auto d1 = buffer[((i1 + 0) * 3 + j1 + 0) * 33 + k1 + 0];
                auto d2 = buffer[((i1 + 0) * 3 + j1 + 1) * 33 + k1 + 0];
                auto d3 = buffer[((i1 + 1) * 3 + j1 + 0) * 33 + k1 + 0];
                auto d4 = buffer[((i1 + 1) * 3 + j1 + 1) * 33 + k1 + 0];
                auto d5 = (buffer[((i1 + 0) * 3 + j1 + 0) * 33 + k1 + 1] - d1) * d0;
                auto d6 = (buffer[((i1 + 0) * 3 + j1 + 1) * 33 + k1 + 1] - d2) * d0;
                auto d7 = (buffer[((i1 + 1) * 3 + j1 + 0) * 33 + k1 + 1] - d3) * d0;
                auto d8 = (buffer[((i1 + 1) * 3 + j1 + 1) * 33 + k1 + 1] - d4) * d0;

                for (auto l1 = 0; l1 < 4; ++l1)
                {
                    auto d9  = 0.125;
                    auto d10 = d1;
                    auto d11 = d2;
                    auto d12 = (d3 - d1) * d9;
                    auto d13 = (d4 - d2) * d9;

                    for (auto i2 = 0; i2 < 8; ++i2)
                    {
                        auto d14 = 0.125;
                        auto d15 = d10;
                        auto d16 = (d11 - d10) * d14;

                        for (auto j2 = 0; j2 < 8; ++j2)
                        {
                            auto iblockstate = AIR;
                            if (d15 > 0.0)
                            {
                                iblockstate = END_STONE;
                            }

                            auto k2 = i2 + i1 * 8;
                            auto l2 = l1 + k1 * 4;
                            auto i3 = j2 + j1 * 8;
                            primer.setBlockState(k2, l2, i3, iblockstate);
                            d15 += d16;
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

void ChunkGeneratorEnd::buildSurfaces(ChunkPrimer &primer)
{
    for (auto i = 0; i < 16; ++i)
    {
        for (auto j = 0; j < 16; ++j)
        {
            auto k            = true;
            auto l            = -1;
            auto iblockstate  = END_STONE;
            auto iblockstate1 = END_STONE;

            for (auto i1 = 127; i1 >= 0; --i1)
            {
                auto iblockstate2 = primer.getBlockState(i, i1, j);
                if (iblockstate2.getMaterial() == Material::AIR)
                {
                    l = -1;
                }
                else if (iblockstate2.getBlock() == Blocks::STONE)
                {
                    if (l == -1)
                    {
                        l = 1;
                        if (i1 >= 0)
                        {
                            primer.setBlockState(i, i1, j, iblockstate);
                        }
                        else
                        {
                            primer.setBlockState(i, i1, j, iblockstate1);
                        }
                    }
                    else if (l > 0)
                    {
                        --l;
                        primer.setBlockState(i, i1, j, iblockstate1);
                    }
                }
            }
        }
    }
}

Chunk ChunkGeneratorEnd::generateChunk(int32_t x, int32_t z)
{
    rand.seed(x * 341873128712 + z * 132897987541);
    ChunkPrimer chunkprimer;
    biomesForGeneration = world->getBiomeProvider().getBiomes(biomesForGeneration, x * 16, z * 16, 16, 16);
    setBlocksInChunk(x, z, chunkprimer);
    buildSurfaces(chunkprimer);
    if (mapFeaturesEnabled)
    {
        endCityGen.generate(world, x, z, chunkprimer);
    }

    Chunk chunk(world, chunkprimer, x, z);
    auto abyte = chunk.getBiomeArray();

    for (int i = 0; i < abyte.size(); ++i)
    {
        abyte[i] = Biome::getIdForBiome(biomesForGeneration[i]);
    }

    chunk.generateSkylightMap();
    return chunk;
}

bool ChunkGeneratorEnd::isIslandChunk(int32_t p_185961_1_, int32_t p_185961_2_)
{
    return (long)p_185961_1_ * (long)p_185961_1_ + (long)p_185961_2_ * (long)p_185961_2_ > 4096L &&
           getIslandHeightValue(p_185961_1_, p_185961_2_, 1, 1) >= 0.0F;
}

void ChunkGeneratorEnd::populate(int32_t x, int32_t z)
{
    BlockFalling::fallInstantly = true;
    BlockPos blockpos(x * 16, 0, z * 16);
    if (mapFeaturesEnabled)
    {
        endCityGen.generateStructure(world, rand, ChunkPos(x, z));
    }

    world->getBiome(blockpos.add(16, 0, 16)).decorate(world, world->rand, blockpos);
    auto i = (long)x * (long)x + (long)z * (long)z;
    if (i > 4096L)
    {
        auto f = getIslandHeightValue(x, z, 1, 1);
        if (f < -20.0F && rand(14) == 0)
        {
            endIslands.generate(world, rand, blockpos.add(rand(16) + 8, 55 + rand(16), rand(16) + 8));
            if (rand(4) == 0)
            {
                endIslands.generate(world, rand, blockpos.add(rand(16) + 8, 55 + rand(16), rand(16) + 8));
            }
        }

        if (getIslandHeightValue(x, z, 1, 1) > 40.0F)
        {
            auto j = rand(5);
            for (auto l1 = 0; l1 < j; ++l1)
            {
                auto i2 = rand(16) + 8;
                auto j2 = rand(16) + 8;
                auto k2 = world->getHeight(blockpos.add(i2, 0, j2)).gety();
                if (k2 > 0)
                {
                    auto k1 = k2 - 1;
                    if (world->isAirBlock(blockpos.add(i2, k1 + 1, j2)) &&
                        world->getBlockState(blockpos.add(i2, k1, j2)).getBlock() == Blocks::END_STONE)
                    {
                        BlockChorusFlower.generatePlant(world, blockpos.add(i2, k1 + 1, j2), rand, 8);
                    }
                }
            }

            if (rand(700) == 0)
            {
                auto l1 = rand(16) + 8;
                auto i2 = rand(16) + 8;
                auto j2 = world->getHeight(blockpos.add(l1, 0, i2)).gety();
                if (j2 > 0)
                {
                    auto k2        = j2 + 3 + rand(7);
                    auto blockpos1 = blockpos.add(l1, k2, i2);
                    (new WorldGenEndGateway()).generate(world, rand, blockpos1);
                    auto tileentity = world->getTileEntity(blockpos1);
                    if (Util:: instanceof <TileEntityEndGateway>(tileentity))
                    {
                        auto tileentityendgateway = (TileEntityEndGateway)tileentity;
                        tileentityendgateway.setExactPosition(spawnPoint);
                    }
                }
            }
        }
    }

    BlockFalling::fallInstantly = false;
}

bool ChunkGeneratorEnd::generateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    return false;
}

std::vector<SpawnListEntry> ChunkGeneratorEnd::getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos)
{
    return world->getBiome(pos).getSpawnableList(creatureType);
}

std::optional<BlockPos> ChunkGeneratorEnd::getNearestStructurePos(World *worldIn, std::string_view structureName,
                                                                  BlockPos &position, bool findUnexplored)
{
    return "EndCity" == structureName ? endCityGen.getNearestStructurePos(worldIn, position, findUnexplored)
                                      : std::nullopt;
}

bool ChunkGeneratorEnd::isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos)
{
    return "EndCity" == structureName ? endCityGen.isInsideStructure(pos) : false;
}

void ChunkGeneratorEnd::recreateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
}

std::vector<double> ChunkGeneratorEnd::getHeights(std::vector<double> &p_185963_1_, int32_t p_185963_2_,
                                                  int32_t p_185963_3_, int32_t p_185963_4_, int32_t p_185963_5_,
                                                  int32_t p_185963_6_, int32_t p_185963_7_)
{
    if (p_185963_1_.empty())
    {
        p_185963_1_.resize(p_185963_5_ * p_185963_6_ * p_185963_7_);
    }

    auto d0 = 684.412;
    auto d1 = 684.412;
    d0 *= 2.0;
    pnr    = perlinNoise1.generateNoiseOctaves(pnr, p_185963_2_, p_185963_3_, p_185963_4_, p_185963_5_, p_185963_6_,
                                               p_185963_7_, d0 / 80.0, 4.277575000000001, d0 / 80.0);
    ar     = lperlinNoise1.generateNoiseOctaves(ar, p_185963_2_, p_185963_3_, p_185963_4_, p_185963_5_, p_185963_6_,
                                                p_185963_7_, d0, 684.412, d0);
    br     = lperlinNoise2.generateNoiseOctaves(br, p_185963_2_, p_185963_3_, p_185963_4_, p_185963_5_, p_185963_6_,
                                                p_185963_7_, d0, 684.412, d0);
    auto i = p_185963_2_ / 2;
    auto j = p_185963_4_ / 2;
    auto k = 0;

    for (auto l = 0; l < p_185963_5_; ++l)
    {
        for (auto i1 = 0; i1 < p_185963_7_; ++i1)
        {
            auto f = getIslandHeightValue(i, j, l, i1);

            for (auto j1 = 0; j1 < p_185963_6_; ++j1)
            {
                auto d2 = ar[k] / 512.0;
                auto d3 = br[k] / 512.0;
                auto d5 = (pnr[k] / 10.0 + 1.0) / 2.0;
                auto d4 = 0.0;
                if (d5 < 0.0)
                {
                    d4 = d2;
                }
                else if (d5 > 1.0)
                {
                    d4 = d3;
                }
                else
                {
                    d4 = d2 + (d3 - d2) * d5;
                }

                d4 -= 8.0;
                d4 += (double)f;
                auto k1 = 2;
                auto d7 = 0.0;
                if (j1 > p_185963_6_ / 2 - k1)
                {
                    d7 = (double)((float)(j1 - (p_185963_6_ / 2 - k1)) / 64.0F);
                    d7 = MathHelper::clamp(d7, 0.0, 1.0);
                    d4 = d4 * (1.0 - d7) + -3000.0 * d7;
                }

                k1 = 8;
                if (j1 < k1)
                {
                    d7 = (double)((float)(k1 - j1) / ((float)k1 - 1.0F));
                    d4 = d4 * (1.0 - d7) + -30.0 * d7;
                }

                p_185963_1_[k] = d4;
                ++k;
            }
        }
    }

    return p_185963_1_;
}

float ChunkGeneratorEnd::getIslandHeightValue(int32_t p_185960_1_, int32_t p_185960_2_, int32_t p_185960_3_,
                                              int32_t p_185960_4_)
{
    auto f  = (float)(p_185960_1_ * 2 + p_185960_3_);
    auto f1 = (float)(p_185960_2_ * 2 + p_185960_4_);
    auto f2 = 100.0F - MathHelper::sqrt(f * f + f1 * f1) * 8.0F;
    if (f2 > 80.0F)
    {
        f2 = 80.0F;
    }

    if (f2 < -100.0F)
    {
        f2 = -100.0F;
    }

    for (auto i = -12; i <= 12; ++i)
    {
        for (auto j = -12; j <= 12; ++j)
        {
            auto k = (long)(p_185960_1_ + i);
            auto l = (long)(p_185960_2_ + j);
            if (k * k + l * l > 4096 && islandNoise.getValue((double)k, (double)l) < -0.8999999761581421)
            {
                auto f3 = MathHelper::positiveModulo(
                              MathHelper::abs((float)k) * 3439.0F + MathHelper::abs((float)l) * 147.0F, 13.0F) +
                          9.0F;
                f        = (float)(p_185960_3_ - i * 2);
                f1       = (float)(p_185960_4_ - j * 2);
                float f4 = 100.0F - MathHelper::sqrt(f * f + f1 * f1) * f3;
                if (f4 > 80.0F)
                {
                    f4 = 80.0F;
                }

                if (f4 < -100.0F)
                {
                    f4 = -100.0F;
                }

                if (f4 > f2)
                {
                    f2 = f4;
                }
            }
        }
    }

    return f2;
}
