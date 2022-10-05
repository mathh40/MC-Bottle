#include "ChunkGeneratorSettings.h"

ChunkGeneratorSettings::Factory ChunkGeneratorSettings::Factory::jsonToFactory(std::string_view p_177865_0_)
{
    if (p_177865_0_.empty())
    {
        return Factory();
    }
    else
    {
        try
        {
            auto j3 = nlohmann::json::parse(p_177865_0_);
            Factory f;
            j3.get_to(f);
            return f;
        }
        catch (Exception var2)
        {
            return Factory();
        }
    }
}

std::string ChunkGeneratorSettings::Factory::toString()
{
    nlohmann::json j = *this;
    return j.dump();
}

ChunkGeneratorSettings::Factory::Factory()
{
    setDefaults();
}

void ChunkGeneratorSettings::Factory::setDefaults()
{
    coordinateScale         = 684.412F;
    heightScale             = 684.412F;
    upperLimitScale         = 512.0F;
    lowerLimitScale         = 512.0F;
    depthNoiseScaleX        = 200.0F;
    depthNoiseScaleZ        = 200.0F;
    depthNoiseScaleExponent = 0.5F;
    mainNoiseScaleX         = 80.0F;
    mainNoiseScaleY         = 160.0F;
    mainNoiseScaleZ         = 80.0F;
    baseSize                = 8.5F;
    stretchY                = 12.0F;
    biomeDepthWeight        = 1.0F;
    biomeDepthOffset        = 0.0F;
    biomeScaleWeight        = 1.0F;
    biomeScaleOffset        = 0.0F;
    seaLevel                = 63;
    useCaves                = true;
    useDungeons             = true;
    dungeonChance           = 8;
    useStrongholds          = true;
    useVillages             = true;
    useMineShafts           = true;
    useTemples              = true;
    useMonuments            = true;
    useMansions             = true;
    useRavines              = true;
    useWaterLakes           = true;
    waterLakeChance         = 4;
    useLavaLakes            = true;
    lavaLakeChance          = 80;
    useLavaOceans           = false;
    fixedBiome              = -1;
    biomeSize               = 4;
    riverSize               = 4;
    dirtSize                = 33;
    dirtCount               = 10;
    dirtMinHeight           = 0;
    dirtMaxHeight           = 256;
    gravelSize              = 33;
    gravelCount             = 8;
    gravelMinHeight         = 0;
    gravelMaxHeight         = 256;
    graniteSize             = 33;
    graniteCount            = 10;
    graniteMinHeight        = 0;
    graniteMaxHeight        = 80;
    dioriteSize             = 33;
    dioriteCount            = 10;
    dioriteMinHeight        = 0;
    dioriteMaxHeight        = 80;
    andesiteSize            = 33;
    andesiteCount           = 10;
    andesiteMinHeight       = 0;
    andesiteMaxHeight       = 80;
    coalSize                = 17;
    coalCount               = 20;
    coalMinHeight           = 0;
    coalMaxHeight           = 128;
    ironSize                = 9;
    ironCount               = 20;
    ironMinHeight           = 0;
    ironMaxHeight           = 64;
    goldSize                = 9;
    goldCount               = 2;
    goldMinHeight           = 0;
    goldMaxHeight           = 32;
    redstoneSize            = 8;
    redstoneCount           = 8;
    redstoneMinHeight       = 0;
    redstoneMaxHeight       = 16;
    diamondSize             = 8;
    diamondCount            = 1;
    diamondMinHeight        = 0;
    diamondMaxHeight        = 16;
    lapisSize               = 7;
    lapisCount              = 1;
    lapisCenterHeight       = 16;
    lapisSpread             = 16;
}

ChunkGeneratorSettings ChunkGeneratorSettings::Factory::build() const
{
    return ChunkGeneratorSettings(*this);
}

ChunkGeneratorSettings::ChunkGeneratorSettings(Factory settingsFactory)
{
    coordinateScale         = settingsFactory.coordinateScale;
    heightScale             = settingsFactory.heightScale;
    upperLimitScale         = settingsFactory.upperLimitScale;
    lowerLimitScale         = settingsFactory.lowerLimitScale;
    depthNoiseScaleX        = settingsFactory.depthNoiseScaleX;
    depthNoiseScaleZ        = settingsFactory.depthNoiseScaleZ;
    depthNoiseScaleExponent = settingsFactory.depthNoiseScaleExponent;
    mainNoiseScaleX         = settingsFactory.mainNoiseScaleX;
    mainNoiseScaleY         = settingsFactory.mainNoiseScaleY;
    mainNoiseScaleZ         = settingsFactory.mainNoiseScaleZ;
    baseSize                = settingsFactory.baseSize;
    stretchY                = settingsFactory.stretchY;
    biomeDepthWeight        = settingsFactory.biomeDepthWeight;
    biomeDepthOffSet        = settingsFactory.biomeDepthOffset;
    biomeScaleWeight        = settingsFactory.biomeScaleWeight;
    biomeScaleOffset        = settingsFactory.biomeScaleOffset;
    seaLevel                = settingsFactory.seaLevel;
    useCaves                = settingsFactory.useCaves;
    useDungeons             = settingsFactory.useDungeons;
    dungeonChance           = settingsFactory.dungeonChance;
    useStrongholds          = settingsFactory.useStrongholds;
    useVillages             = settingsFactory.useVillages;
    useMineShafts           = settingsFactory.useMineShafts;
    useTemples              = settingsFactory.useTemples;
    useMonuments            = settingsFactory.useMonuments;
    useMansions             = settingsFactory.useMansions;
    useRavines              = settingsFactory.useRavines;
    useWaterLakes           = settingsFactory.useWaterLakes;
    waterLakeChance         = settingsFactory.waterLakeChance;
    useLavaLakes            = settingsFactory.useLavaLakes;
    lavaLakeChance          = settingsFactory.lavaLakeChance;
    useLavaOceans           = settingsFactory.useLavaOceans;
    fixedBiome              = settingsFactory.fixedBiome;
    biomeSize               = settingsFactory.biomeSize;
    riverSize               = settingsFactory.riverSize;
    dirtSize                = settingsFactory.dirtSize;
    dirtCount               = settingsFactory.dirtCount;
    dirtMinHeight           = settingsFactory.dirtMinHeight;
    dirtMaxHeight           = settingsFactory.dirtMaxHeight;
    gravelSize              = settingsFactory.gravelSize;
    gravelCount             = settingsFactory.gravelCount;
    gravelMinHeight         = settingsFactory.gravelMinHeight;
    gravelMaxHeight         = settingsFactory.gravelMaxHeight;
    graniteSize             = settingsFactory.graniteSize;
    graniteCount            = settingsFactory.graniteCount;
    graniteMinHeight        = settingsFactory.graniteMinHeight;
    graniteMaxHeight        = settingsFactory.graniteMaxHeight;
    dioriteSize             = settingsFactory.dioriteSize;
    dioriteCount            = settingsFactory.dioriteCount;
    dioriteMinHeight        = settingsFactory.dioriteMinHeight;
    dioriteMaxHeight        = settingsFactory.dioriteMaxHeight;
    andesiteSize            = settingsFactory.andesiteSize;
    andesiteCount           = settingsFactory.andesiteCount;
    andesiteMinHeight       = settingsFactory.andesiteMinHeight;
    andesiteMaxHeight       = settingsFactory.andesiteMaxHeight;
    coalSize                = settingsFactory.coalSize;
    coalCount               = settingsFactory.coalCount;
    coalMinHeight           = settingsFactory.coalMinHeight;
    coalMaxHeight           = settingsFactory.coalMaxHeight;
    ironSize                = settingsFactory.ironSize;
    ironCount               = settingsFactory.ironCount;
    ironMinHeight           = settingsFactory.ironMinHeight;
    ironMaxHeight           = settingsFactory.ironMaxHeight;
    goldSize                = settingsFactory.goldSize;
    goldCount               = settingsFactory.goldCount;
    goldMinHeight           = settingsFactory.goldMinHeight;
    goldMaxHeight           = settingsFactory.goldMaxHeight;
    redstoneSize            = settingsFactory.redstoneSize;
    redstoneCount           = settingsFactory.redstoneCount;
    redstoneMinHeight       = settingsFactory.redstoneMinHeight;
    redstoneMaxHeight       = settingsFactory.redstoneMaxHeight;
    diamondSize             = settingsFactory.diamondSize;
    diamondCount            = settingsFactory.diamondCount;
    diamondMinHeight        = settingsFactory.diamondMinHeight;
    diamondMaxHeight        = settingsFactory.diamondMaxHeight;
    lapisSize               = settingsFactory.lapisSize;
    lapisCount              = settingsFactory.lapisCount;
    lapisCenterHeight       = settingsFactory.lapisCenterHeight;
    lapisSpread             = settingsFactory.lapisSpread;
}

bool operator==(const ChunkGeneratorSettings::Factory &lhs,
                const ChunkGeneratorSettings::Factory &chunkgeneratorsettings$factory)
{
    if (lhs.andesiteCount != chunkgeneratorsettings$factory.andesiteCount)
    {
        return false;
    }
    else if (lhs.andesiteMaxHeight != chunkgeneratorsettings$factory.andesiteMaxHeight)
    {
        return false;
    }
    else if (lhs.andesiteMinHeight != chunkgeneratorsettings$factory.andesiteMinHeight)
    {
        return false;
    }
    else if (lhs.andesiteSize != chunkgeneratorsettings$factory.andesiteSize)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.baseSize != lhs.baseSize)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.biomeDepthOffset != lhs.biomeDepthOffset)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.biomeDepthWeight != lhs.biomeDepthWeight)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.biomeScaleOffset != lhs.biomeScaleOffset)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.biomeScaleWeight != lhs.biomeScaleWeight)
    {
        return false;
    }
    else if (lhs.biomeSize != chunkgeneratorsettings$factory.biomeSize)
    {
        return false;
    }
    else if (lhs.coalCount != chunkgeneratorsettings$factory.coalCount)
    {
        return false;
    }
    else if (lhs.coalMaxHeight != chunkgeneratorsettings$factory.coalMaxHeight)
    {
        return false;
    }
    else if (lhs.coalMinHeight != chunkgeneratorsettings$factory.coalMinHeight)
    {
        return false;
    }
    else if (lhs.coalSize != chunkgeneratorsettings$factory.coalSize)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.coordinateScale != lhs.coordinateScale)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.depthNoiseScaleExponent != lhs.depthNoiseScaleExponent)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.depthNoiseScaleX != lhs.depthNoiseScaleX)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.depthNoiseScaleZ != lhs.depthNoiseScaleZ)
    {
        return false;
    }
    else if (lhs.diamondCount != chunkgeneratorsettings$factory.diamondCount)
    {
        return false;
    }
    else if (lhs.diamondMaxHeight != chunkgeneratorsettings$factory.diamondMaxHeight)
    {
        return false;
    }
    else if (lhs.diamondMinHeight != chunkgeneratorsettings$factory.diamondMinHeight)
    {
        return false;
    }
    else if (lhs.diamondSize != chunkgeneratorsettings$factory.diamondSize)
    {
        return false;
    }
    else if (lhs.dioriteCount != chunkgeneratorsettings$factory.dioriteCount)
    {
        return false;
    }
    else if (lhs.dioriteMaxHeight != chunkgeneratorsettings$factory.dioriteMaxHeight)
    {
        return false;
    }
    else if (lhs.dioriteMinHeight != chunkgeneratorsettings$factory.dioriteMinHeight)
    {
        return false;
    }
    else if (lhs.dioriteSize != chunkgeneratorsettings$factory.dioriteSize)
    {
        return false;
    }
    else if (lhs.dirtCount != chunkgeneratorsettings$factory.dirtCount)
    {
        return false;
    }
    else if (lhs.dirtMaxHeight != chunkgeneratorsettings$factory.dirtMaxHeight)
    {
        return false;
    }
    else if (lhs.dirtMinHeight != chunkgeneratorsettings$factory.dirtMinHeight)
    {
        return false;
    }
    else if (lhs.dirtSize != chunkgeneratorsettings$factory.dirtSize)
    {
        return false;
    }
    else if (lhs.dungeonChance != chunkgeneratorsettings$factory.dungeonChance)
    {
        return false;
    }
    else if (lhs.fixedBiome != chunkgeneratorsettings$factory.fixedBiome)
    {
        return false;
    }
    else if (lhs.goldCount != chunkgeneratorsettings$factory.goldCount)
    {
        return false;
    }
    else if (lhs.goldMaxHeight != chunkgeneratorsettings$factory.goldMaxHeight)
    {
        return false;
    }
    else if (lhs.goldMinHeight != chunkgeneratorsettings$factory.goldMinHeight)
    {
        return false;
    }
    else if (lhs.goldSize != chunkgeneratorsettings$factory.goldSize)
    {
        return false;
    }
    else if (lhs.graniteCount != chunkgeneratorsettings$factory.graniteCount)
    {
        return false;
    }
    else if (lhs.graniteMaxHeight != chunkgeneratorsettings$factory.graniteMaxHeight)
    {
        return false;
    }
    else if (lhs.graniteMinHeight != chunkgeneratorsettings$factory.graniteMinHeight)
    {
        return false;
    }
    else if (lhs.graniteSize != chunkgeneratorsettings$factory.graniteSize)
    {
        return false;
    }
    else if (lhs.gravelCount != chunkgeneratorsettings$factory.gravelCount)
    {
        return false;
    }
    else if (lhs.gravelMaxHeight != chunkgeneratorsettings$factory.gravelMaxHeight)
    {
        return false;
    }
    else if (lhs.gravelMinHeight != chunkgeneratorsettings$factory.gravelMinHeight)
    {
        return false;
    }
    else if (lhs.gravelSize != chunkgeneratorsettings$factory.gravelSize)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.heightScale != lhs.heightScale)
    {
        return false;
    }
    else if (lhs.ironCount != chunkgeneratorsettings$factory.ironCount)
    {
        return false;
    }
    else if (lhs.ironMaxHeight != chunkgeneratorsettings$factory.ironMaxHeight)
    {
        return false;
    }
    else if (lhs.ironMinHeight != chunkgeneratorsettings$factory.ironMinHeight)
    {
        return false;
    }
    else if (lhs.ironSize != chunkgeneratorsettings$factory.ironSize)
    {
        return false;
    }
    else if (lhs.lapisCenterHeight != chunkgeneratorsettings$factory.lapisCenterHeight)
    {
        return false;
    }
    else if (lhs.lapisCount != chunkgeneratorsettings$factory.lapisCount)
    {
        return false;
    }
    else if (lhs.lapisSize != chunkgeneratorsettings$factory.lapisSize)
    {
        return false;
    }
    else if (lhs.lapisSpread != chunkgeneratorsettings$factory.lapisSpread)
    {
        return false;
    }
    else if (lhs.lavaLakeChance != chunkgeneratorsettings$factory.lavaLakeChance)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.lowerLimitScale != lhs.lowerLimitScale)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.mainNoiseScaleX != lhs.mainNoiseScaleX)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.mainNoiseScaleY != lhs.mainNoiseScaleY)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.mainNoiseScaleZ != lhs.mainNoiseScaleZ)
    {
        return false;
    }
    else if (lhs.redstoneCount != chunkgeneratorsettings$factory.redstoneCount)
    {
        return false;
    }
    else if (lhs.redstoneMaxHeight != chunkgeneratorsettings$factory.redstoneMaxHeight)
    {
        return false;
    }
    else if (lhs.redstoneMinHeight != chunkgeneratorsettings$factory.redstoneMinHeight)
    {
        return false;
    }
    else if (lhs.redstoneSize != chunkgeneratorsettings$factory.redstoneSize)
    {
        return false;
    }
    else if (lhs.riverSize != chunkgeneratorsettings$factory.riverSize)
    {
        return false;
    }
    else if (lhs.seaLevel != chunkgeneratorsettings$factory.seaLevel)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.stretchY != lhs.stretchY)
    {
        return false;
    }
    else if (chunkgeneratorsettings$factory.upperLimitScale != lhs.upperLimitScale)
    {
        return false;
    }
    else if (lhs.useCaves != chunkgeneratorsettings$factory.useCaves)
    {
        return false;
    }
    else if (lhs.useDungeons != chunkgeneratorsettings$factory.useDungeons)
    {
        return false;
    }
    else if (lhs.useLavaLakes != chunkgeneratorsettings$factory.useLavaLakes)
    {
        return false;
    }
    else if (lhs.useLavaOceans != chunkgeneratorsettings$factory.useLavaOceans)
    {
        return false;
    }
    else if (lhs.useMineShafts != chunkgeneratorsettings$factory.useMineShafts)
    {
        return false;
    }
    else if (lhs.useRavines != chunkgeneratorsettings$factory.useRavines)
    {
        return false;
    }
    else if (lhs.useStrongholds != chunkgeneratorsettings$factory.useStrongholds)
    {
        return false;
    }
    else if (lhs.useTemples != chunkgeneratorsettings$factory.useTemples)
    {
        return false;
    }
    else if (lhs.useMonuments != chunkgeneratorsettings$factory.useMonuments)
    {
        return false;
    }
    else if (lhs.useMansions != chunkgeneratorsettings$factory.useMansions)
    {
        return false;
    }
    else if (lhs.useVillages != chunkgeneratorsettings$factory.useVillages)
    {
        return false;
    }
    else if (lhs.useWaterLakes != chunkgeneratorsettings$factory.useWaterLakes)
    {
        return false;
    }
    else
    {
        return lhs.waterLakeChance == chunkgeneratorsettings$factory.waterLakeChance;
    }
    return false;
}

bool operator!=(const ChunkGeneratorSettings::Factory &lhs, const ChunkGeneratorSettings::Factory &rhs)
{
    return !(lhs == rhs);
}
