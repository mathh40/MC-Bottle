#include "BiomeProvider.h"

#include "../../util/ReportedException.h"
#include "WorldType.h"

BiomeProvider::BiomeProvider(WorldInfo info)
    : BiomeProvider(info.getSeed(), info.getTerrainType(), info.getGeneratorOptions())
{
}

std::vector<Biome *> BiomeProvider::getBiomesToSpawnIn() const
{
    return biomesToSpawnIn;
}

Biome *BiomeProvider::getBiome(BlockPos &pos) const
{
    return getBiome(pos, nullptr);
}

Biome *BiomeProvider::getBiome(BlockPos &pos, Biome *defaultBiome) const
{
    return biomeCache.getBiome(pos.getx(), pos.getz(), defaultBiome);
}

float BiomeProvider::getTemperatureAtHeight(float p_76939_1_, int32_t p_76939_2_)
{
    return p_76939_1_;
}

std::vector<Biome *> BiomeProvider::getBiomesForGeneration(std::vector<Biome *> biomes, int32_t x, int32_t z,
                                                           int32_t width, int32_t height)
{
    if (biomes.empty || biomes.size() < width * height)
    {
        biomes.resize(width * height);
    }

    auto aint = genBiomes.getInts(x, z, width, height);

    try
    {
        for (auto i = 0; i < width * height; ++i)
        {
            biomes[i] = Biome::getBiome(aint[i], Biomes::DEFAULT);
        }

        return biomes;
    }
    catch (Throwable var10)
    {
        CrashReport crashreport                 = CrashReport.makeCrashReport(var10, "Invalid Biome id");
        CrashReportCategory crashreportcategory = crashreport.makeCategory("RawBiomeBlock");
        crashreportcategory.addCrashSection("biomes[] size", biomes.size());
        crashreportcategory.addCrashSection("x", x);
        crashreportcategory.addCrashSection("z", z);
        crashreportcategory.addCrashSection("w", width);
        crashreportcategory.addCrashSection("h", height);
        throw ReportedException(crashreport);
    }
}

std::vector<Biome *> BiomeProvider::getBiomes(std::vector<Biome *> oldBiomeList, int32_t x, int32_t z, int32_t width,
                                              int32_t depth)
{
    return getBiomes(oldBiomeList, x, z, width, depth, true);
}

std::vector<Biome *> BiomeProvider::getBiomes(std::vector<Biome *> listToReuse, int32_t x, int32_t z, int32_t width,
                                              int32_t length, bool cacheFlag)
{
    if (listToReuse.empty() || listToReuse.size() < width * length)
    {
        listToReuse.resize(width * length);
    }

    if (cacheFlag && width == 16 && length == 16 && (x & 15) == 0 && (z & 15) == 0)
    {
        auto abiome = biomeCache.getCachedBiomes(x, z);
        listToReuse.assign(abiome.begin(), abiome.end());
        return listToReuse;
    }
    else
    {
        auto aint = biomeIndexLayer.getInts(x, z, width, length);

        for (auto i = 0; i < width * length; ++i)
        {
            listToReuse[i] = Biome::getBiome(aint[i], Biomes::DEFAULT);
        }

        return listToReuse;
    }
}

bool BiomeProvider::areBiomesViable(int32_t x, int32_t z, int32_t radius, std::vector<Biome *> allowed)
{
    auto i    = x - radius >> 2;
    auto j    = z - radius >> 2;
    auto k    = x + radius >> 2;
    auto l    = z + radius >> 2;
    auto i1   = k - i + 1;
    auto j1   = l - j + 1;
    auto aint = genBiomes.getInts(i, j, i1, j1);

    try
    {
        for (auto k1 = 0; k1 < i1 * j1; ++k1)
        {
            auto biome = Biome::getBiome(aint[k1]);
            auto ite   = std::find_if(allowed.begin(), allowed.end(), [](const Biome *lhs) { return lhs == biome; });
            if (ite == allowed.end())
            {
                return false;
            }
        }

        return true;
    }
    catch (Throwable var15)
    {
        CrashReport crashreport                 = CrashReport.makeCrashReport(var15, "Invalid Biome id");
        CrashReportCategory crashreportcategory = crashreport.makeCategory("Layer");
        crashreportcategory.addCrashSection("Layer", genBiomes.toString());
        crashreportcategory.addCrashSection("x", x);
        crashreportcategory.addCrashSection("z", z);
        crashreportcategory.addCrashSection("radius", radius);
        crashreportcategory.addCrashSection("allowed", allowed);
        throw ReportedException(crashreport);
    }
}

std::optional<BlockPos> BiomeProvider::findBiomePosition(int32_t x, int32_t z, int32_t range,
                                                         std::vector<Biome *> biomes, pcg32 &random)
{
    auto i                           = x - range >> 2;
    auto j                           = z - range >> 2;
    auto k                           = x + range >> 2;
    auto l                           = z + range >> 2;
    auto i1                          = k - i + 1;
    auto j1                          = l - j + 1;
    auto aint                        = genBiomes.getInts(i, j, i1, j1);
    std::optional<BlockPos> blockpos = std::nullopt;
    auto k1                          = 0;

    for (auto l1 = 0; l1 < i1 * j1; ++l1)
    {
        auto i2    = i + l1 % i1 << 2;
        auto j2    = j + l1 / i1 << 2;
        auto biome = Biome::getBiome(aint[l1]);
        auto ite   = std::find_if(biomes.begin(), biomes.end(), [](const Biome *lhs) { return lhs == biome; });
        if (ite != biomes.end() && (blockpos == std::nullopt || random(k1 + 1) == 0))
        {
            blockpos = BlockPos(i2, 0, j2);
            ++k1;
        }
    }

    return blockpos;
}

void BiomeProvider::cleanupCache()
{
    biomeCache.cleanupCache();
}

bool BiomeProvider::isFixedBiome()
{
    return settings.fixedBiome >= 0;
}

Biome *BiomeProvider::getFixedBiome()
{
    return settings.fixedBiome >= 0 ? Biome::getBiomeForId(settings.fixedBiome) : nullptr;
}

BiomeProvider::BiomeProvider()
    : biomeCache(*this), biomesToSpawnIn{Biomes::FOREST,       Biomes::PLAINS, Biomes::TAIGA,       Biomes::TAIGA_HILLS,
                                         Biomes::FOREST_HILLS, Biomes::JUNGLE, Biomes::JUNGLE_HILLS}
{
}

BiomeProvider::BiomeProvider(int64_t seed, WorldType worldTypeIn, std::string_view options) : BiomeProvider()
{
    if (worldTypeIn == WorldType::CUSTOMIZED && !options.empty())
    {
        settings = ChunkGeneratorSettings.Factory.jsonToFactory(options).build();
    }

    GenLayer[] agenlayer = GenLayer.initializeAllBiomeGenerators(seed, worldTypeIn, settings);
    genBiomes            = agenlayer[0];
    biomeIndexLayer      = agenlayer[1];
}
