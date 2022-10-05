#include "WorldProvider.h"

#include "biome/BiomeProviderSingle.h"
#include "gen/ChunkGeneratorDebug.h"
#include "gen/ChunkGeneratorFlat.h"
#include "gen/ChunkGeneratorOverworld.h"
#include "gen/FlatGeneratorInfo.h"

std::array<float, 8> WorldProvider::MOON_PHASE_FACTORS = {1.0F, 0.75F, 0.5F, 0.25F, 0.0F, 0.25F, 0.5F, 0.75F};

void WorldProvider::setWorld(World *worldIn)
{
    world             = worldIn;
    terrainType       = worldIn->getWorldInfo().getTerrainType();
    generatorSettings = worldIn->getWorldInfo().getGeneratorOptions();
    init();
    generateLightBrightnessTable();
}

void WorldProvider::generateLightBrightnessTable()
{
    float f = 0.0F;

    for (auto i = 0; i <= 15; ++i)
    {
        auto f1                 = 1.0F - i / 15.0F;
        lightBrightnessTable[i] = (1.0F - f1) / (f1 * 3.0F + 1.0F) * 1.0F + 0.0F;
    }
}

void WorldProvider::init()
{
    SkyLight       = true;
    auto worldtype = world->getWorldInfo().getTerrainType();
    if (worldtype == WorldType::FLAT)
    {
        FlatGeneratorInfo flatgeneratorinfo =
            FlatGeneratorInfo.createFlatGeneratorFromString(world->getWorldInfo().getGeneratorOptions());
        biomeProvider = BiomeProviderSingle(Biome.getBiome(flatgeneratorinfo.getBiome(), Biomes.DEFAULT));
    }
    else if (worldtype == WorldType.DEBUG_ALL_BLOCK_STATES)
    {
        biomeProvider = BiomeProviderSingle(Biomes.PLAINS);
    }
    else
    {
        biomeProvider = BiomeProvider(world->getWorldInfo());
    }
}

IChunkGenerator &WorldProvider::createChunkGenerator()
{
    if (terrainType == WorldType::FLAT)
    {
        return ChunkGeneratorFlat(world, world->getSeed(), world->getWorldInfo().isMapFeaturesEnabled(),
                                  generatorSettings);
    }
    else if (terrainType == WorldType::DEBUG_ALL_BLOCK_STATES)
    {
        return ChunkGeneratorDebug(world);
    }
    else
    {
        return terrainType == WorldType::CUSTOMIZED
                   ? ChunkGeneratorOverworld(world, world->getSeed(), world->getWorldInfo().isMapFeaturesEnabled(),
                                             generatorSettings)
                   : ChunkGeneratorOverworld(world, world->getSeed(), world->getWorldInfo().isMapFeaturesEnabled(),
                                             generatorSettings);
    }
}

bool WorldProvider::canCoordinateBeSpawn(int32_t x, int32_t z)
{
    BlockPos blockpos(x, 0, z);
    if (world->getBiome(blockpos).ignorePlayerSpawnSuitability())
    {
        return true;
    }
    else
    {
        return world->getGroundAboveSeaLevel(blockpos).getBlock() == Blocks::GRASS;
    }
}

float WorldProvider::calculateCelestialAngle(int64_t worldTime, float partialTicks) const
{
    auto i = (worldTime % 24000);
    auto f = (i + partialTicks) / 24000.0F - 0.25F;
    if (f < 0.0F)
    {
        ++f;
    }

    if (f > 1.0F)
    {
        --f;
    }

    auto f1 = 1.0F - ((MathHelper::cos(f * 3.141592653589793) + 1.0) / 2.0);
    f += (f1 - f) / 3.0F;
    return f;
}

int32_t WorldProvider::getMoonPhase(int64_t worldTime) const
{
    return (worldTime / 24000 % 8 + 8) % 8;
}

bool WorldProvider::isSurfaceWorld() const
{
    return true;
}

std::optional<std::array<float, 4>> WorldProvider::calcSunriseSunsetColors(float celestialAngle, float partialTicks)
{
    auto f  = 0.4F;
    auto f1 = MathHelper::cos(celestialAngle * 6.2831855F) - 0.0F;
    auto f2 = -0.0F;
    if (f1 >= -0.4F && f1 <= 0.4F)
    {
        auto f3 = (f1 - -0.0F) / 0.4F * 0.5F + 0.5F;
        auto f4 = 1.0F - (1.0F - MathHelper::sin(f3 * 3.1415927F)) * 0.99F;
        f4 *= f4;
        colorsSunriseSunset[0] = f3 * 0.3F + 0.7F;
        colorsSunriseSunset[1] = f3 * f3 * 0.7F + 0.2F;
        colorsSunriseSunset[2] = f3 * f3 * 0.0F + 0.2F;
        colorsSunriseSunset[3] = f4;
        return colorsSunriseSunset;
    }
    else
    {
        return std::nullopt;
    }
}

Vec3d WorldProvider::getFogColor(float p_76562_1_, float p_76562_2_) const
{
    auto f  = MathHelper::cos(p_76562_1_ * 6.2831855F) * 2.0F + 0.5F;
    f       = MathHelper::clamp(f, 0.0F, 1.0F);
    auto f1 = 0.7529412F;
    auto f2 = 0.84705883F;
    auto f3 = 1.0F;
    f1 *= f * 0.94F + 0.06F;
    f2 *= f * 0.94F + 0.06F;
    f3 *= f * 0.91F + 0.09F;
    return Vec3d(f1, f2, f3);
}

bool WorldProvider::canRespawnHere() const
{
    return true;
}

float WorldProvider::getCloudHeight() const
{
    return 128.0F;
}

bool WorldProvider::isSkyColored() const
{
    return true;
}

std::optional<BlockPos> WorldProvider::getSpawnCoordinate() const
{
    return std::nullopt;
}

int32_t WorldProvider::getAverageGroundLevel() const
{
    return terrainType == WorldType::FLAT ? 4 : world->getSeaLevel() + 1;
}

double WorldProvider::getVoidFogYFactor() const
{
    return terrainType == WorldType::FLAT ? 1.0 : 0.03125;
}

bool WorldProvider::doesXZShowFog(int32_t x, int32_t z) const
{
    return false;
}

BiomeProvider &WorldProvider::getBiomeProvider() const
{
    return biomeProvider;
}

bool WorldProvider::doesWaterVaporize() const
{
    return dWaterVaporize;
}

bool WorldProvider::hasSkyLight() const
{
    return SkyLight;
}

bool WorldProvider::isNether() const
{
    return nether;
}

std::array<float, 16> WorldProvider::getLightBrightnessTable() const
{
    return lightBrightnessTable;
}

WorldBorder WorldProvider::createWorldBorder()
{
    return WorldBorder();
}

void WorldProvider::onPlayerAdded(EntityPlayerMP *player)
{
}

void WorldProvider::onPlayerRemoved(EntityPlayerMP *player)
{
}

void WorldProvider::onWorldSave()
{
}

void WorldProvider::onWorldUpdateEntities()
{
}

bool WorldProvider::canDropChunk(int32_t x, int32_t z)
{
    return true;
}
