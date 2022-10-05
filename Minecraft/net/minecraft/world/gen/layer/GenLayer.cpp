#include "GenLayer.h"

#include "GenLayerAddIsland.h"
#include "GenLayerAddMushroomIsland.h"
#include "GenLayerAddSnow.h"
#include "GenLayerBiome.h"
#include "GenLayerBiomeEdge.h"
#include "GenLayerDeepOcean.h"
#include "GenLayerEdge.h"
#include "GenLayerFuzzyZoom.h"
#include "GenLayerHills.h"
#include "GenLayerIsland.h"
#include "GenLayerRareBiome.h"
#include "GenLayerRemoveTooMuchOcean.h"
#include "GenLayerRiver.h"
#include "GenLayerRiverInit.h"
#include "GenLayerRiverMix.h"
#include "GenLayerShore.h"
#include "GenLayerSmooth.h"
#include "GenLayerVoronoiZoom.h"
#include "GenLayerZoom.h"
#include "WorldType.h"
#include "biome/Biome.h"
#include "gen/ChunkGeneratorSettings.h"

GenLayer::GenLayer(int64_t baseSeedIn)
{
    baseSeed = baseSeedIn;
    baseSeed *= baseSeed * 6364136223846793005L + 1442695040888963407L;
    baseSeed += baseSeedIn;
    baseSeed *= baseSeed * 6364136223846793005L + 1442695040888963407L;
    baseSeed += baseSeedIn;
    baseSeed *= baseSeed * 6364136223846793005L + 1442695040888963407L;
    baseSeed += baseSeedIn;
}

void GenLayer::initWorldGenSeed(int64_t seed)
{
    worldGenSeed = seed;
    if (parent != nullptr)
    {
        parent->initWorldGenSeed(seed);
    }

    worldGenSeed *= worldGenSeed * 6364136223846793005L + 1442695040888963407L;
    worldGenSeed += baseSeed;
    worldGenSeed *= worldGenSeed * 6364136223846793005L + 1442695040888963407L;
    worldGenSeed += baseSeed;
    worldGenSeed *= worldGenSeed * 6364136223846793005L + 1442695040888963407L;
    worldGenSeed += baseSeed;
}

void GenLayer::initChunkSeed(int64_t chunkSeedIn, int64_t chunkSeedIn2)
{
    chunkSeed = worldGenSeed;
    chunkSeed *= chunkSeed * 6364136223846793005L + 1442695040888963407L;
    chunkSeed += chunkSeedIn;
    chunkSeed *= chunkSeed * 6364136223846793005L + 1442695040888963407L;
    chunkSeed += chunkSeedIn2;
    chunkSeed *= chunkSeed * 6364136223846793005L + 1442695040888963407L;
    chunkSeed += chunkSeedIn;
    chunkSeed *= chunkSeed * 6364136223846793005L + 1442695040888963407L;
    chunkSeed += chunkSeedIn2;
}

std::vector<std::shared_ptr<GenLayer>> GenLayer::initializeAllBiomeGenerators(
    int64_t seed, WorldType p_180781_2_, std::optional<ChunkGeneratorSettings> p_180781_3_)
{
    auto genlayer                   = std::make_shared<GenLayerIsland>(1L);
    genlayer                        = std::make_shared<GenLayerFuzzyZoom>(2000L, genlayer);
    auto genlayeraddisland          = std::make_shared<GenLayerAddIsland>(1L, genlayer);
    auto genlayerzoom               = std::make_shared<GenLayerZoom>(2001L, genlayeraddisland);
    auto genlayeraddisland1         = std::make_shared<GenLayerAddIsland>(2L, genlayerzoom);
    genlayeraddisland1              = std::make_shared<GenLayerAddIsland>(50L, genlayeraddisland1);
    genlayeraddisland1              = std::make_shared<GenLayerAddIsland>(70L, genlayeraddisland1);
    auto genlayerremovetoomuchocean = std::make_shared<GenLayerRemoveTooMuchOcean>(2L, genlayeraddisland1);
    auto genlayeraddsnow            = std::make_shared<GenLayerAddSnow>(2L, genlayerremovetoomuchocean);
    auto genlayeraddisland2         = std::make_shared<GenLayerAddIsland>(3L, genlayeraddsnow);
    auto genlayeredge       = std::make_shared<GenLayerEdge>(2L, genlayeraddisland2, GenLayerEdge::Mode::COOL_WARM);
    genlayeredge            = std::make_shared<GenLayerEdge>(2L, genlayeredge, GenLayerEdge::Mode::HEAT_ICE);
    genlayeredge            = std::make_shared<GenLayerEdge>(3L, genlayeredge, GenLayerEdge::Mode::SPECIAL);
    auto genlayerzoom1      = std::make_shared<GenLayerZoom>(2002L, genlayeredge);
    genlayerzoom1           = std::make_shared<GenLayerZoom>(2003L, genlayerzoom1);
    auto genlayeraddisland3 = std::make_shared<GenLayerAddIsland>(4L, genlayerzoom1);
    auto genlayeraddmushroomisland = std::make_shared<GenLayerAddMushroomIsland>(5L, genlayeraddisland3);
    auto genlayerdeepocean         = std::make_shared<GenLayerDeepOcean>(4L, genlayeraddmushroomisland);
    auto genlayer4                 = GenLayerZoom::magnify(1000L, genlayerdeepocean, 0);
    auto i                         = 4;
    auto j                         = i;
    if (p_180781_3_.has_value())
    {
        i = p_180781_3_.value().biomeSize;
        j = p_180781_3_.value().riverSize;
    }

    if (p_180781_2_ == WorldType::LARGE_BIOMES)
    {
        i = 6;
    }

    auto lvt_7_1_          = GenLayerZoom::magnify(1000L, genlayer4, 0);
    auto genlayerriverinit = std::make_shared<GenLayerRiverInit>(100L, lvt_7_1_);
    auto lvt_8_1_          = std::make_shared<GenLayerBiome>(200L, genlayer4, p_180781_2_, p_180781_3_.value());
    auto genlayer6         = GenLayerZoom::magnify(1000L, lvt_8_1_, 2);
    auto genlayerbiomeedge = std::make_shared<GenLayerBiomeEdge>(1000L, genlayer6);
    auto lvt_9_1_          = GenLayerZoom::magnify(1000L, genlayerriverinit, 2);
    auto genlayerhills     = std::make_shared<GenLayerHills>(1000L, genlayerbiomeedge, lvt_9_1_);
    auto genlayer5         = GenLayerZoom::magnify(1000L, genlayerriverinit, 2);
    genlayer5              = GenLayerZoom::magnify(1000L, genlayer5, j);
    auto genlayerriver     = std::make_shared<GenLayerRiver>(1L, genlayer5);
    auto genlayersmooth    = std::make_shared<GenLayerSmooth>(1000L, genlayerriver);
    auto genlayerhills     = std::make_shared<GenLayerRareBiome>(1001L, genlayerhills);

    for (int k = 0; k < i; ++k)
    {
        genlayerhills = std::make_shared<GenLayerZoom>((long)(1000 + k), (GenLayer)genlayerhills);
        if (k == 0)
        {
            genlayerhills = std::make_shared<GenLayerAddIsland>(3L, (GenLayer)genlayerhills);
        }

        if (k == 1 || i == 1)
        {
            genlayerhills = std::make_shared<GenLayerShore>(1000L, (GenLayer)genlayerhills);
        }
    }

    auto genlayersmooth1  = std::make_shared<GenLayerSmooth>(1000L, (GenLayer)genlayerhills);
    auto genlayerrivermix = std::make_shared<GenLayerRiverMix>(100L, genlayersmooth1, genlayersmooth);
    auto genlayer3        = std::make_shared<GenLayerVoronoiZoom>(10L, genlayerrivermix);
    genlayerrivermix->initWorldGenSeed(seed);
    genlayer3->initWorldGenSeed(seed);
    return {genlayerrivermix, genlayer3, genlayerrivermix};
}

int32_t GenLayer::nextInt(int32_t value)
{
    int32_t i = (int32_t)((chunkSeed >> 24) % (int64_t)value);
    if (i < 0)
    {
        i += value;
    }

    chunkSeed *= chunkSeed * 6364136223846793005L + 1442695040888963407L;
    chunkSeed += worldGenSeed;
    return i;
}

bool GenLayer::biomesEqualOrMesaPlateau(int32_t biomeIDA, int32_t biomeIDB)
{
    if (biomeIDA == biomeIDB)
    {
        return true;
    }
    else
    {
        auto biome  = Biome::getBiome(biomeIDA);
        auto biome1 = Biome::getBiome(biomeIDB);
        if (biome != nullptr && biome1 != nullptr)
        {
            if (biome != Biomes::MESA_ROCK && biome != Biomes::MESA_CLEAR_ROCK)
            {
                return biome == biome1 || biome->getBiomeClass() == biome1->getBiomeClass();
            }
            else
            {
                return biome1 == Biomes::MESA_ROCK || biome1 == Biomes::MESA_CLEAR_ROCK;
            }
        }
        else
        {
            return false;
        }
    }
}

bool GenLayer::isBiomeOceanic(int32_t biomeID)
{
    auto biome = Biome::getBiome(biomeID);
    return biome == Biomes::OCEAN || biome == Biomes::DEEP_OCEAN || biome == Biomes::FROZEN_OCEAN;
}

int32_t GenLayer::selectRandom(const std::vector<int32_t> &p_151619_1_)
{
    return p_151619_1_[nextInt(p_151619_1_.size())];
}

int32_t GenLayer::selectModeOrRandom(int32_t p_151617_1_, int32_t p_151617_2_, int32_t p_151617_3_, int32_t p_151617_4_)
{
    if (p_151617_2_ == p_151617_3_ && p_151617_3_ == p_151617_4_)
    {
        return p_151617_2_;
    }
    else if (p_151617_1_ == p_151617_2_ && p_151617_1_ == p_151617_3_)
    {
        return p_151617_1_;
    }
    else if (p_151617_1_ == p_151617_2_ && p_151617_1_ == p_151617_4_)
    {
        return p_151617_1_;
    }
    else if (p_151617_1_ == p_151617_3_ && p_151617_1_ == p_151617_4_)
    {
        return p_151617_1_;
    }
    else if (p_151617_1_ == p_151617_2_ && p_151617_3_ != p_151617_4_)
    {
        return p_151617_1_;
    }
    else if (p_151617_1_ == p_151617_3_ && p_151617_2_ != p_151617_4_)
    {
        return p_151617_1_;
    }
    else if (p_151617_1_ == p_151617_4_ && p_151617_2_ != p_151617_3_)
    {
        return p_151617_1_;
    }
    else if (p_151617_2_ == p_151617_3_ && p_151617_1_ != p_151617_4_)
    {
        return p_151617_2_;
    }
    else if (p_151617_2_ == p_151617_4_ && p_151617_1_ != p_151617_3_)
    {
        return p_151617_2_;
    }
    else
    {
        return p_151617_3_ == p_151617_4_ && p_151617_1_ != p_151617_2_
                   ? p_151617_3_
                   : selectRandom({p_151617_1_, p_151617_2_, p_151617_3_, p_151617_4_});
    }
}
