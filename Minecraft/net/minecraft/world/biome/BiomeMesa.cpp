#include "BiomeMesa.h"

#include "../../../../../spdlog/spdlog/fmt/bundled/format.h"
#include "../../util/Util.h"
#include "../../util/math/MathHelper.h"

#include <random>

IBlockState *BiomeMesa::COARSE_DIRT =
    Blocks.DIRT.getDefaultState().withProperty(BlockDirt.VARIANT, BlockDirt.DirtType.COARSE_DIRT);
IBlockState *BiomeMesa::GRASS                 = Blocks.GRASS.getDefaultState();
IBlockState *BiomeMesa::HARDENED_CLAY         = Blocks.HARDENED_CLAY.getDefaultState();
IBlockState *BiomeMesa::STAINED_HARDENED_CLAY = Blocks.STAINED_HARDENED_CLAY.getDefaultState();
IBlockState *BiomeMesa::ORANGE_STAINED_HARDENED_CLAY =
    STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.ORANGE);
IBlockState *BiomeMesa::RED_SAND =
    Blocks.SAND.getDefaultState().withProperty(BlockSand.VARIANT, BlockSand.EnumType.RED_SAND);

void BiomeMesa::Decorator::generateOres(World *worldIn, pcg32 &random)
{
    BiomeDecorator::generateOres(worldIn, random);
    genStandardOre1(worldIn, random, 20, goldGen, 32, 80);
}

BiomeMesa::BiomeMesa(bool p_i46704_1_, bool p_i46704_2_, BiomeProperties properties) : Biome(properties)
{
    brycePillars = p_i46704_1_;
    hasForest    = p_i46704_2_;
    spawnableCreatureList.clear();
    topBlock                   = RED_SAND;
    fillerBlock                = STAINED_HARDENED_CLAY;
    decorator.treesPerChunk    = -999;
    decorator.deadBushPerChunk = 20;
    decorator.reedsPerChunk    = 3;
    decorator.cactiPerChunk    = 5;
    decorator.flowersPerChunk  = 0;
    spawnableCreatureList.clear();
    if (p_i46704_2_)
    {
        decorator.treesPerChunk = 5;
    }
}

BiomeDecorator BiomeMesa::createBiomeDecorator()
{
    return Decorator();
}

void BiomeMesa::generateBands(int64_t p_150619_1_)
{
    std::fill(clayBands.begin(), clayBands.end(), HARDENED_CLAY);
    pcg32 random(p_150619_1_);
    clayBandsOffsetNoise = new NoiseGeneratorPerlin(random, 1);

    for (auto i2 = 0; i2 < 64; ++i2)
    {
        i2 += random(5) + 1;
        if (i2 < 64)
        {
            clayBands[i2] = ORANGE_STAINED_HARDENED_CLAY;
        }
    }

    auto i2 = random(4) + 2;

    for (auto j2 = 0; j2 < i2; ++j2)
    {
        auto l2 = random(3) + 1;
        auto k3 = random(64);

        for (auto j4 = 0; k3 + j4 < 64 && j4 < l2; ++j4)
        {
            clayBands[k3 + j4] = STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.YELLOW);
        }
    }

    auto j2 = random(4) + 2;

    for (auto l2 = 0; l2 < j2; ++l2)
    {
        auto k3 = random(3) + 2;
        auto j4 = random(64);

        for (auto k4 = 0; j4 + k4 < 64 && k4 < k3; ++k4)
        {
            clayBands[j4 + k4] = STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.BROWN);
        }
    }

    auto l2 = random(4) + 2;

    for (auto k3 = 0; k3 < l2; ++k3)
    {
        auto j4 = random(3) + 1;
        auto k4 = random(64);

        for (auto j1 = 0; k4 + j1 < 64 && j1 < j4; ++j1)
        {
            clayBands[k4 + j1] = STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.RED);
        }
    }

    auto k3 = random(3) + 3;
    auto j4 = 0;

    for (auto k4 = 0; k4 < k3; ++k4)
    {
        int i5 = true;
        j4 += random(16) + 4;

        for (auto k1 = 0; j4 + k1 < 64 && k1 < 1; ++k1)
        {
            clayBands[j4 + k1] = STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.WHITE);

            if (j4 + k1 > 1 && Util::RandomizerWithSentinelShift(random))
            {
                clayBands[j4 + k1 - 1] = STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.SILVER);
            }

            if (j4 + k1 < 63 && Util::RandomizerWithSentinelShift(random))
            {
                clayBands[j4 + k1 + 1] = STAINED_HARDENED_CLAY.withProperty(BlockColored.COLOR, EnumDyeColor.SILVER);
            }
        }
    }
}

IBlockState *BiomeMesa::getBand(int32_t p_180629_1_, int32_t p_180629_2_, int32_t p_180629_3_)
{
    auto i = MathHelper::round(clayBandsOffsetNoise.getValue((double)p_180629_1_ / 512.0, (double)p_180629_1_ / 512.0) *
                               2.0);
    return clayBands[(p_180629_2_ + i + 64) % 64];
}

WorldGenAbstractTree BiomeMesa::getRandomTreeFeature(pcg32 &rand)
{
    return TREE_FEATURE;
}

int32_t BiomeMesa::getFoliageColorAtPos(BlockPos &pos)
{
    return 10387789;
}

int32_t BiomeMesa::getGrassColorAtPos(BlockPos &pos)
{
    return 9470285;
}

void BiomeMesa::genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                                 double noiseVal)
{
    if (clayBands.empty() || worldSeed != worldIn->getSeed())
    {
        generateBands(worldIn->getSeed());
    }

    if (pillarNoise == nullptr || pillarRoofNoise == nullptr || worldSeed != worldIn->getSeed())
    {
        pcg32 random    = pcg32(worldSeed);
        pillarNoise     = new NoiseGeneratorPerlin(random, 4);
        pillarRoofNoise = new NoiseGeneratorPerlin(random, 1);
    }

    worldSeed = worldIn->getSeed();
    double d4 = 0.0;
    if (brycePillars)
    {
        auto k1 = (x & -16) + (z & 15);
        auto l1 = (z & -16) + (x & 15);
        auto d0 =
            MathHelper::min(MathHelper::abs(noiseVal), pillarNoise.getValue((double)k1 * 0.25, (double)l1 * 0.25));
        if (d0 > 0.0)
        {
            auto d1 = 0.001953125;
            auto d2 = MathHelper::abs(pillarRoofNoise.getValue((double)k1 * 0.001953125, (double)l1 * 0.001953125));
            d4      = d0 * d0 * 2.5;
            auto d3 = MathHelper::ceil(d2 * 50.0) + 14.0;
            if (d4 > d3)
            {
                d4 = d3;
            }

            d4 += 64.0;
        }
    }

    auto k1                   = x & 15;
    auto l1                   = z & 15;
    auto i2                   = worldIn->getSeaLevel();
    IBlockState *iblockstate  = STAINED_HARDENED_CLAY;
    IBlockState *iblockstate3 = fillerBlock;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto k     = (int)(noiseVal / 3.0 + 3.0 + dist(rand) * 0.25);
    auto flag  = MathHelper::cos(noiseVal / 3.0 * 3.141592653589793) > 0.0;
    auto l     = -1;
    auto flag1 = false;
    auto i1    = 0;

    for (auto j1 = 255; j1 >= 0; --j1)
    {
        if (chunkPrimerIn.getBlockState(l1, j1, k1).getMaterial() == Material::AIR && j1 < (int)d4)
        {
            chunkPrimerIn.setBlockState(l1, j1, k1, STONE);
        }

        if (j1 <= rand(5))
        {
            chunkPrimerIn.setBlockState(l1, j1, k1, BEDROCK);
        }
        else if (i1 < 15 || brycePillars)
        {
            IBlockState *iblockstate1 = chunkPrimerIn.getBlockState(l1, j1, k1);
            if (iblockstate1->getMaterial() == Material::AIR)
            {
                l = -1;
            }
            else if (iblockstate1->getBlock() == Blocks.STONE)
            {
                if (l == -1)
                {
                    flag1 = false;
                    if (k <= 0)
                    {
                        iblockstate  = AIR;
                        iblockstate3 = STONE;
                    }
                    else if (j1 >= i2 - 4 && j1 <= i2 + 1)
                    {
                        iblockstate  = STAINED_HARDENED_CLAY;
                        iblockstate3 = fillerBlock;
                    }

                    if (j1 < i2 && (iblockstate == nullptr || iblockstate->getMaterial() == Material::AIR))
                    {
                        iblockstate = WATER;
                    }

                    l = k + MathHelper::max(0, j1 - i2);
                    if (j1 >= i2 - 1)
                    {
                        if (hasForest && j1 > 86 + k * 2)
                        {
                            if (flag)
                            {
                                chunkPrimerIn.setBlockState(l1, j1, k1, COARSE_DIRT);
                            }
                            else
                            {
                                chunkPrimerIn.setBlockState(l1, j1, k1, GRASS);
                            }
                        }
                        else if (j1 > i2 + 3 + k)
                        {
                            IBlockState *iblockstate2;
                            if (j1 >= 64 && j1 <= 127)
                            {
                                if (flag)
                                {
                                    iblockstate2 = HARDENED_CLAY;
                                }
                                else
                                {
                                    iblockstate2 = getBand(x, j1, z);
                                }
                            }
                            else
                            {
                                iblockstate2 = ORANGE_STAINED_HARDENED_CLAY;
                            }

                            chunkPrimerIn.setBlockState(l1, j1, k1, iblockstate2);
                        }
                        else
                        {
                            chunkPrimerIn.setBlockState(l1, j1, k1, topBlock);
                            flag1 = true;
                        }
                    }
                    else
                    {
                        chunkPrimerIn.setBlockState(l1, j1, k1, iblockstate3);
                        if (iblockstate3->getBlock() == Blocks.STAINED_HARDENED_CLAY)
                        {
                            chunkPrimerIn.setBlockState(l1, j1, k1, ORANGE_STAINED_HARDENED_CLAY);
                        }
                    }
                }
                else if (l > 0)
                {
                    --l;
                    if (flag1)
                    {
                        chunkPrimerIn.setBlockState(l1, j1, k1, ORANGE_STAINED_HARDENED_CLAY);
                    }
                    else
                    {
                        chunkPrimerIn.setBlockState(l1, j1, k1, getBand(x, j1, z));
                    }
                }

                ++i1;
            }
        }
    }
}
