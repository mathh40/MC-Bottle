#include "ChunkGeneratorHell.h"

#include "../../../../../spdlog/spdlog/fmt/bundled/format.h"
#include "World.h"
#include "chunk/ChunkPrimer.h"

IBlockState *ChunkGeneratorHell::AIR        = Blocks::AIR.getDefaultState();
IBlockState *ChunkGeneratorHell::NETHERRACK = Blocks::NETHERRACK.getDefaultState();
IBlockState *ChunkGeneratorHell::BEDROCK    = Blocks::BEDROCK.getDefaultState();
IBlockState *ChunkGeneratorHell::LAVA       = Blocks::LAVA.getDefaultState();
IBlockState *ChunkGeneratorHell::GRAVEL     = Blocks::GRAVEL.getDefaultState();
IBlockState *ChunkGeneratorHell::SOUL_SAND  = Blocks::SOUL_SAND.getDefaultState();

ChunkGeneratorHell::ChunkGeneratorHell(World *worldIn, bool p_i45637_2_, int64_t seed)
{
    quartzGen = new WorldGenMinable(Blocks.QUARTZ_ORE.getDefaultState(), 14, BlockMatcher.forBlock(Blocks.NETHERRACK));
    magmaGen  = new WorldGenMinable(Blocks.MAGMA.getDefaultState(), 33, BlockMatcher.forBlock(Blocks.NETHERRACK));
    lavaTrapGen                   = new WorldGenHellLava(Blocks.FLOWING_LAVA, true);
    hellSpringGen                 = new WorldGenHellLava(Blocks.FLOWING_LAVA, false);
    brownMushroomFeature          = new WorldGenBush(Blocks.BROWN_MUSHROOM);
    redMushroomFeature            = new WorldGenBush(Blocks.RED_MUSHROOM);
    genNetherBridge               = new MapGenNetherBridge();
    genNetherCaves                = new MapGenCavesHell();
    world                         = worldIn;
    bgenerateStructures           = p_i45637_2_;
    rand                          = pcg32(seed);
    lperlinNoise1                 = new NoiseGeneratorOctaves(rand, 16);
    lperlinNoise2                 = new NoiseGeneratorOctaves(rand, 16);
    perlinNoise1                  = new NoiseGeneratorOctaves(rand, 8);
    slowsandGravelNoiseGen        = new NoiseGeneratorOctaves(rand, 4);
    netherrackExculsivityNoiseGen = new NoiseGeneratorOctaves(rand, 4);
    scaleNoise                    = new NoiseGeneratorOctaves(rand, 10);
    depthNoise                    = new NoiseGeneratorOctaves(rand, 16);
    worldIn->setSeaLevel(63);
}

std::vector<SpawnListEntry> ChunkGeneratorHell::getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos)
{
    if (creatureType == EnumCreatureType::MONSTER)
    {
        if (genNetherBridge.isInsideStructure(pos))
        {
            return genNetherBridge.getSpawnList();
        }

        if (genNetherBridge.isPositionInStructure(world, pos) &&
            world->getBlockState(pos.down()).getBlock() == Blocks::NETHER_BRICK)
        {
            return genNetherBridge.getSpawnList();
        }
    }

    auto biome = world->getBiome(pos);
    return biome.getSpawnableList(creatureType);
}

std::optional<BlockPos> ChunkGeneratorHell::getNearestStructurePos(World *worldIn, std::string_view structureName,
                                                                   BlockPos &position, bool findUnexplored)
{
    return "Fortress" == structureName ? genNetherBridge.getNearestStructurePos(worldIn, position, findUnexplored)
                                       : std::nullopt;
}

bool ChunkGeneratorHell::isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos)
{
    return "Fortress" == structureName ? genNetherBridge.isInsideStructure(pos) : false;
}

void ChunkGeneratorHell::recreateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    genNetherBridge.generate(world, x, z, nullptr);
}

void ChunkGeneratorHell::prepareHeights(int32_t p_185936_1_, int32_t p_185936_2_, ChunkPrimer &primer)
{
    auto j = world->getSeaLevel() / 2 + 1;
    buffer = getHeights(buffer, p_185936_1_ * 4, 0, p_185936_2_ * 4, 5, 17, 5);

    for (auto j1 = 0; j1 < 4; ++j1)
    {
        for (auto k1 = 0; k1 < 4; ++k1)
        {
            for (auto l1 = 0; l1 < 16; ++l1)
            {
                auto d0 = 0.125;
                auto d1 = buffer[((j1 + 0) * 5 + k1 + 0) * 17 + l1 + 0];
                auto d2 = buffer[((j1 + 0) * 5 + k1 + 1) * 17 + l1 + 0];
                auto d3 = buffer[((j1 + 1) * 5 + k1 + 0) * 17 + l1 + 0];
                auto d4 = buffer[((j1 + 1) * 5 + k1 + 1) * 17 + l1 + 0];
                auto d5 = (buffer[((j1 + 0) * 5 + k1 + 0) * 17 + l1 + 1] - d1) * d0;
                auto d6 = (buffer[((j1 + 0) * 5 + k1 + 1) * 17 + l1 + 1] - d2) * d0;
                auto d7 = (buffer[((j1 + 1) * 5 + k1 + 0) * 17 + l1 + 1] - d3) * d0;
                auto d8 = (buffer[((j1 + 1) * 5 + k1 + 1) * 17 + l1 + 1] - d4) * d0;

                for (auto i2 = 0; i2 < 8; ++i2)
                {
                    auto d9  = 0.25;
                    auto d10 = d1;
                    auto d11 = d2;
                    auto d12 = (d3 - d1) * d9;
                    auto d13 = (d4 - d2) * d9;

                    for (auto j2 = 0; j2 < 4; ++j2)
                    {
                        auto d14 = d9;
                        auto d15 = d10;
                        auto d16 = (d11 - d10) * d14;

                        for (auto k2 = 0; k2 < 4; ++k2)
                        {
                            IBlockState *iblockstate = nullptr;
                            if (l1 * 8 + i2 < j)
                            {
                                iblockstate = LAVA;
                            }

                            if (d15 > 0.0)
                            {
                                iblockstate = NETHERRACK;
                            }

                            auto l2 = j2 + j1 * 4;
                            auto i3 = i2 + l1 * 8;
                            auto j3 = k2 + k1 * 4;
                            primer->setBlockState(l2, i3, j3, iblockstate);
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

void ChunkGeneratorHell::buildSurfaces(int32_t p_185937_1_, int32_t p_185937_2_, ChunkPrimer &primer)
{
    auto i        = world->getSeaLevel() + 1;
    auto d0       = 0.03125;
    slowsandNoise = slowsandGravelNoiseGen.generateNoiseOctaves(slowsandNoise, p_185937_1_ * 16, p_185937_2_ * 16, 0,
                                                                16, 16, 1, 0.03125, 0.03125, 1.0);
    gravelNoise = slowsandGravelNoiseGen.generateNoiseOctaves(gravelNoise, p_185937_1_ * 16, 109, p_185937_2_ * 16, 16,
                                                              1, 16, 0.03125, 1.0, 0.03125);
    depthBuffer = netherrackExculsivityNoiseGen.generateNoiseOctaves(depthBuffer, p_185937_1_ * 16, p_185937_2_ * 16, 0,
                                                                     16, 16, 1, 0.0625, 0.0625, 0.0625);

    for (auto j = 0; j < 16; ++j)
    {
        for (auto k = 0; k < 16; ++k)
        {
            std::uniform_real_distribution<double> dis(0.0, 1.0);
            auto flag         = slowsandNoise[j + k * 16] + dis(rand) * 0.2 > 0.0;
            auto flag1        = gravelNoise[j + k * 16] + dis(rand) * 0.2 > 0.0;
            auto l            = (int)(depthBuffer[j + k * 16] / 3.0 + 3.0 + dis(rand) * 0.25);
            auto i1           = -1;
            auto iblockstate  = NETHERRACK;
            auto iblockstate1 = NETHERRACK;

            for (auto j1 = 127; j1 >= 0; --j1)
            {
                if (j1 < 127 - rand(5) && j1 > rand(5))
                {
                    auto iblockstate2 = primer->getBlockState(k, j1, j);
                    if (iblockstate2.getBlock() != null && iblockstate2.getMaterial() != Material::AIR)
                    {
                        if (iblockstate2.getBlock() == Blocks::NETHERRACK)
                        {
                            if (i1 == -1)
                            {
                                if (l <= 0)
                                {
                                    iblockstate  = AIR;
                                    iblockstate1 = NETHERRACK;
                                }
                                else if (j1 >= i - 4 && j1 <= i + 1)
                                {
                                    iblockstate  = NETHERRACK;
                                    iblockstate1 = NETHERRACK;
                                    if (flag1)
                                    {
                                        iblockstate  = GRAVEL;
                                        iblockstate1 = NETHERRACK;
                                    }

                                    if (flag)
                                    {
                                        iblockstate  = SOUL_SAND;
                                        iblockstate1 = SOUL_SAND;
                                    }
                                }

                                if (j1 < i && (iblockstate == null || iblockstate.getMaterial() == Material::AIR))
                                {
                                    iblockstate = LAVA;
                                }

                                i1 = l;
                                if (j1 >= i - 1)
                                {
                                    primer.setBlockState(k, j1, j, iblockstate);
                                }
                                else
                                {
                                    primer.setBlockState(k, j1, j, iblockstate1);
                                }
                            }
                            else if (i1 > 0)
                            {
                                --i1;
                                primer.setBlockState(k, j1, j, iblockstate1);
                            }
                        }
                    }
                    else
                    {
                        i1 = -1;
                    }
                }
                else
                {
                    primer.setBlockState(k, j1, j, BEDROCK);
                }
            }
        }
    }
}

Chunk ChunkGeneratorHell::generateChunk(int32_t x, int32_t z)
{
    rand.seed((long)x * 341873128712L + (long)z * 132897987541L);
    ChunkPrimer chunkprimer;
    prepareHeights(x, z, chunkprimer);
    buildSurfaces(x, z, chunkprimer);
    genNetherCaves.generate(world, x, z, chunkprimer);
    if (bgenerateStructures)
    {
        genNetherBridge.generate(world, x, z, chunkprimer);
    }

    Chunk chunk(world, chunkprimer, x, z);
    auto abiome = world->getBiomeProvider().getBiomes(std::vector<Biome *>(), x * 16, z * 16, 16, 16);
    auto abyte  = chunk.getBiomeArray();

    for (auto i = 0; i < abyte.size(); ++i)
    {
        abyte[i] = Biome::getIdForBiome(abiome[i]);
    }

    chunk.resetRelightChecks();
    return chunk;
}

void ChunkGeneratorHell::populate(int32_t x, int32_t z)
{
    BlockFalling::fallInstantly = true;
    auto i                      = x * 16;
    auto j                      = z * 16;
    BlockPos blockpos(i, 0, j);
    auto biome = world->getBiome(blockpos.add(16, 0, 16));
    ChunkPos chunkpos(x, z);
    genNetherBridge.generateStructure(world, rand, chunkpos);

    int i2;
    for (i2 = 0; i2 < 8; ++i2)
    {
        hellSpringGen.generate(world, rand, blockpos.add(rand(16) + 8, rand(120) + 4, rand(16) + 8));
    }

    for (i2 = 0; i2 < rand(rand(10) + 1) + 1; ++i2)
    {
        fireFeature.generate(world, rand, blockpos.add(rand(16) + 8, rand(120) + 4, rand(16) + 8));
    }

    for (i2 = 0; i2 < rand(rand(10) + 1); ++i2)
    {
        lightGemGen.generate(world, rand, blockpos.add(rand(16) + 8, rand(120) + 4, rand(16) + 8));
    }

    for (i2 = 0; i2 < 10; ++i2)
    {
        hellPortalGen.generate(world, rand, blockpos.add(rand(16) + 8, rand(128), rand(16) + 8));
    }

    Util::RandomizerWithSentinelShift rn;
    if (rn(rand))
    {
        brownMushroomFeature.generate(world, rand, blockpos.add(rand(16) + 8, rand(128), rand(16) + 8));
    }

    if (rn(rand))
    {
        redMushroomFeature.generate(world, rand, blockpos.add(rand(16) + 8, rand(128), rand(16) + 8));
    }

    for (i2 = 0; i2 < 16; ++i2)
    {
        quartzGen.generate(world, rand, blockpos.add(rand(16), rand(108) + 10, rand(16)));
    }

    i2 = world->getSeaLevel() / 2 + 1;

    int j2;
    for (j2 = 0; j2 < 4; ++j2)
    {
        magmaGen.generate(world, rand, blockpos.add(rand(16), i2 - 5 + rand(10), rand(16)));
    }

    for (j2 = 0; j2 < 16; ++j2)
    {
        lavaTrapGen.generate(world, rand, blockpos.add(rand(16), rand(108) + 10, rand(16)));
    }

    biome.decorate(world, rand, BlockPos(i, 0, j));
    BlockFalling::fallInstantly = false;
}

bool ChunkGeneratorHell::generateStructures(Chunk &chunkIn, int32_t x, int32_t z)
{
    return false;
}

std::vector<double> ChunkGeneratorHell::getHeights(std::vector<double> &p_185938_1_, int32_t p_185938_2_,
                                                   int32_t p_185938_3_, int32_t p_185938_4_, int32_t p_185938_5_,
                                                   int32_t p_185938_6_, int32_t p_185938_7_)
{
    if (p_185938_1_.empty())
    {
        p_185938_1_.resize(p_185938_5_ * p_185938_6_ * p_185938_7_);
    }

    auto d0    = 684.412;
    auto d1    = 2053.236;
    noiseData4 = scaleNoise.generateNoiseOctaves(noiseData4, p_185938_2_, p_185938_3_, p_185938_4_, p_185938_5_, 1,
                                                 p_185938_7_, 1.0, 0.0, 1.0);
    dr  = depthNoise.generateNoiseOctaves(dr, p_185938_2_, p_185938_3_, p_185938_4_, p_185938_5_, 1, p_185938_7_, 100.0,
                                          0.0, 100.0);
    pnr = perlinNoise1.generateNoiseOctaves(pnr, p_185938_2_, p_185938_3_, p_185938_4_, p_185938_5_, p_185938_6_,
                                            p_185938_7_, 8.555150000000001, 34.2206, 8.555150000000001);
    ar  = lperlinNoise1.generateNoiseOctaves(ar, p_185938_2_, p_185938_3_, p_185938_4_, p_185938_5_, p_185938_6_,
                                             p_185938_7_, 684.412, 2053.236, 684.412);
    br  = lperlinNoise2.generateNoiseOctaves(br, p_185938_2_, p_185938_3_, p_185938_4_, p_185938_5_, p_185938_6_,
                                             p_185938_7_, 684.412, 2053.236, 684.412);
    auto i = 0;
    std::vector<double> adouble(p_185938_6_);

    for (auto j = 0; j < p_185938_6_; ++j)
    {
        adouble[j] = MathHelper::cos((double)j * 3.141592653589793 * 6.0 / (double)p_185938_6_) * 2.0;
        auto d2    = (double)j;
        if (j > p_185938_6_ / 2)
        {
            d2 = (double)(p_185938_6_ - 1 - j);
        }

        if (d2 < 4.0)
        {
            d2 = 4.0 - d2;
            adouble[j] -= d2 * d2 * d2 * 10.0;
        }
    }

    for (auto j = 0; j < p_185938_5_; ++j)
    {
        for (auto i1 = 0; i1 < p_185938_7_; ++i1)
        {
            auto d3 = 0.0;

            for (auto k = 0; k < p_185938_6_; ++k)
            {
                auto d4 = adouble[k];
                auto d5 = ar[i] / 512.0;
                auto d6 = br[i] / 512.0;
                auto d7 = (pnr[i] / 10.0 + 1.0) / 2.0;
                auto d8 = 0.0;
                if (d7 < 0.0)
                {
                    d8 = d5;
                }
                else if (d7 > 1.0)
                {
                    d8 = d6;
                }
                else
                {
                    d8 = d5 + (d6 - d5) * d7;
                }

                d8 -= d4;
                double d10;
                if (k > p_185938_6_ - 4)
                {
                    d10 = (double)((float)(k - (p_185938_6_ - 4)) / 3.0F);
                    d8  = d8 * (1.0 - d10) + -10.0 * d10;
                }

                if ((double)k < 0.0)
                {
                    d10 = (0.0 - (double)k) / 4.0;
                    d10 = MathHelper::clamp(d10, 0.0, 1.0);
                    d8  = d8 * (1.0 - d10) + -10.0 * d10;
                }

                p_185938_1_[i] = d8;
                ++i;
            }
        }
    }

    return p_185938_1_;
}
