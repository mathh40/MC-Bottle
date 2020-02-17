#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include "Util.h"
#include "biome/Biome.h"

class ChunkGeneratorSettings
{
public:
    class Factory
    {
    public:
        float coordinateScale = 684.412F;
        float heightScale = 684.412F;
        float upperLimitScale = 512.0F;
        float lowerLimitScale = 512.0F;
        float depthNoiseScaleX = 200.0F;
        float depthNoiseScaleZ = 200.0F;
        float depthNoiseScaleExponent = 0.5F;
        float mainNoiseScaleX = 80.0F;
        float mainNoiseScaleY = 160.0F;
        float mainNoiseScaleZ = 80.0F;
        float baseSize = 8.5F;
        float stretchY = 12.0F;
        float biomeDepthWeight = 1.0F;
        float biomeDepthOffset;
        float biomeScaleWeight = 1.0F;
        float biomeScaleOffset;
        int32_t seaLevel = 63;
        bool useCaves = true;
        bool useDungeons = true;
        int32_t dungeonChance = 8;
        bool useStrongholds = true;
        bool useVillages = true;
        bool useMineShafts = true;
        bool useTemples = true;
        bool useMonuments = true;
        bool useMansions = true;
        bool useRavines = true;
        bool useWaterLakes = true;
        int32_t waterLakeChance = 4;
        bool useLavaLakes = true;
        int32_t lavaLakeChance = 80;
        bool useLavaOceans;
        int32_t fixedBiome = -1;
        int32_t biomeSize = 4;
        int32_t riverSize = 4;
        int32_t dirtSize = 33;
        int32_t dirtCount = 10;
        int32_t dirtMinHeight;
        int32_t dirtMaxHeight = 256;
        int32_t gravelSize = 33;
        int32_t gravelCount = 8;
        int32_t gravelMinHeight;
        int32_t gravelMaxHeight = 256;
        int32_t graniteSize = 33;
        int32_t graniteCount = 10;
        int32_t graniteMinHeight;
        int32_t graniteMaxHeight = 80;
        int32_t dioriteSize = 33;
        int32_t dioriteCount = 10;
        int32_t dioriteMinHeight;
        int32_t dioriteMaxHeight = 80;
        int32_t andesiteSize = 33;
        int32_t andesiteCount = 10;
        int32_t andesiteMinHeight;
        int32_t andesiteMaxHeight = 80;
        int32_t coalSize = 17;
        int32_t coalCount = 20;
        int32_t coalMinHeight;
        int32_t coalMaxHeight = 128;
        int32_t ironSize = 9;
        int32_t ironCount = 20;
        int32_t ironMinHeight;
        int32_t ironMaxHeight = 64;
        int32_t goldSize = 9;
        int32_t goldCount = 2;
        int32_t goldMinHeight;
        int32_t goldMaxHeight = 32;
        int32_t redstoneSize = 8;
        int32_t redstoneCount = 8;
        int32_t redstoneMinHeight;
        int32_t redstoneMaxHeight = 16;
        int32_t diamondSize = 8;
        int32_t diamondCount = 1;
        int32_t diamondMinHeight;
        int32_t diamondMaxHeight = 16;
        int32_t lapisSize = 7;
        int32_t lapisCount = 1;
        int32_t lapisCenterHeight = 16;
        int32_t lapisSpread = 16;

        static Factory jsonToFactory(std::string_view p_177865_0_);
        std::string toString();
        Factory();
        void setDefaults();
        friend bool operator==(const Factory& lhs, const Factory& p);
        friend bool operator!=(const Factory& lhs, const Factory& rhs);
        ChunkGeneratorSettings build() const;
    };

    float coordinateScale;
    float heightScale;
    float upperLimitScale;
    float lowerLimitScale;
    float depthNoiseScaleX;
    float depthNoiseScaleZ;
    float depthNoiseScaleExponent;
    float mainNoiseScaleX;
    float mainNoiseScaleY;
    float mainNoiseScaleZ;
    float baseSize;
    float stretchY;
    float biomeDepthWeight;
    float biomeDepthOffSet;
    float biomeScaleWeight;
    float biomeScaleOffset;
    int32_t seaLevel;
    bool useCaves;
    bool useDungeons;
    int32_t dungeonChance;
    bool useStrongholds;
    bool useVillages;
    bool useMineShafts;
    bool useTemples;
    bool useMonuments;
    bool useMansions;
    bool useRavines;
    bool useWaterLakes;
    int32_t waterLakeChance;
    bool useLavaLakes;
    int32_t lavaLakeChance;
    bool useLavaOceans;
    int32_t fixedBiome;
    int32_t biomeSize;
    int32_t riverSize;
    int32_t dirtSize;
    int32_t dirtCount;
    int32_t dirtMinHeight;
    int32_t dirtMaxHeight;
    int32_t gravelSize;
    int32_t gravelCount;
    int32_t gravelMinHeight;
    int32_t gravelMaxHeight;
    int32_t graniteSize;
    int32_t graniteCount;
    int32_t graniteMinHeight;
    int32_t graniteMaxHeight;
    int32_t dioriteSize;
    int32_t dioriteCount;
    int32_t dioriteMinHeight;
    int32_t dioriteMaxHeight;
    int32_t andesiteSize;
    int32_t andesiteCount;
    int32_t andesiteMinHeight;
    int32_t andesiteMaxHeight;
    int32_t coalSize;
    int32_t coalCount;
    int32_t coalMinHeight;
    int32_t coalMaxHeight;
    int32_t ironSize;
    int32_t ironCount;
    int32_t ironMinHeight;
    int32_t ironMaxHeight;
    int32_t goldSize;
    int32_t goldCount;
    int32_t goldMinHeight;
    int32_t goldMaxHeight;
    int32_t redstoneSize;
    int32_t redstoneCount;
    int32_t redstoneMinHeight;
    int32_t redstoneMaxHeight;
    int32_t diamondSize;
    int32_t diamondCount;
    int32_t diamondMinHeight;
    int32_t diamondMaxHeight;
    int32_t lapisSize;
    int32_t lapisCount;
    int32_t lapisCenterHeight;
    int32_t lapisSpread;
private:
    ChunkGeneratorSettings(Factory settingsFactory);
};

namespace std 
{
    template<>
    class hash<ChunkGeneratorSettings::Factory>
    {
    public:
        size_t operator()(const ChunkGeneratorSettings::Factory& s) const
        {
            size_t i = s.coordinateScale == 0.0F ? 0 : Util::float2ul(s.coordinateScale);
            i = 31 * i + (s.heightScale == 0.0F ? 0 : Util::float2ul(s.heightScale));
            i = 31 * i + (s.upperLimitScale == 0.0F ? 0 : Util::float2ul(s.upperLimitScale));
            i = 31 * i + (s.lowerLimitScale == 0.0F ? 0 : Util::float2ul(s.lowerLimitScale));
            i = 31 * i + (s.depthNoiseScaleX == 0.0F ? 0 : Util::float2ul(s.depthNoiseScaleX));
            i = 31 * i + (s.depthNoiseScaleZ == 0.0F ? 0 : Util::float2ul(s.depthNoiseScaleZ));
            i = 31 * i + (s.depthNoiseScaleExponent == 0.0F ? 0 : Util::float2ul(s.depthNoiseScaleExponent));
            i = 31 * i + (s.mainNoiseScaleX == 0.0F ? 0 : Util::float2ul(s.mainNoiseScaleX));
            i = 31 * i + (s.mainNoiseScaleY == 0.0F ? 0 : Util::float2ul(s.mainNoiseScaleY));
            i = 31 * i + (s.mainNoiseScaleZ == 0.0F ? 0 : Util::float2ul(s.mainNoiseScaleZ));
            i = 31 * i + (s.baseSize == 0.0F ? 0 : Util::float2ul(s.baseSize));
            i = 31 * i + (s.stretchY == 0.0F ? 0 : Util::float2ul(s.stretchY));
            i = 31 * i + (s.biomeDepthWeight == 0.0F ? 0 : Util::float2ul(s.biomeDepthWeight));
            i = 31 * i + (s.biomeDepthOffset == 0.0F ? 0 : Util::float2ul(s.biomeDepthOffset));
            i = 31 * i + (s.biomeScaleWeight == 0.0F ? 0 : Util::float2ul(s.biomeScaleWeight));
            i = 31 * i + (s.biomeScaleOffset == 0.0F ? 0 : Util::float2ul(s.biomeScaleOffset));
            i = 31 * i + s.seaLevel;
            i = 31 * i + (s.useCaves ? 1 : 0);
            i = 31 * i + (s.useDungeons ? 1 : 0);
            i = 31 * i + s.dungeonChance;
            i = 31 * i + (s.useStrongholds ? 1 : 0);
            i = 31 * i + (s.useVillages ? 1 : 0);
            i = 31 * i + (s.useMineShafts ? 1 : 0);
            i = 31 * i + (s.useTemples ? 1 : 0);
            i = 31 * i + (s.useMonuments ? 1 : 0);
            i = 31 * i + (s.useMansions ? 1 : 0);
            i = 31 * i + (s.useRavines ? 1 : 0);
            i = 31 * i + (s.useWaterLakes ? 1 : 0);
            i = 31 * i + s.waterLakeChance;
            i = 31 * i + (s.useLavaLakes ? 1 : 0);
            i = 31 * i + s.lavaLakeChance;
            i = 31 * i + (s.useLavaOceans ? 1 : 0);
            i = 31 * i + s.fixedBiome;
            i = 31 * i + s.biomeSize;
            i = 31 * i + s.riverSize;
            i = 31 * i + s.dirtSize;
            i = 31 * i + s.dirtCount;
            i = 31 * i + s.dirtMinHeight;
            i = 31 * i + s.dirtMaxHeight;
            i = 31 * i + s.gravelSize;
            i = 31 * i + s.gravelCount;
            i = 31 * i + s.gravelMinHeight;
            i = 31 * i + s.gravelMaxHeight;
            i = 31 * i + s.graniteSize;
            i = 31 * i + s.graniteCount;
            i = 31 * i + s.graniteMinHeight;
            i = 31 * i + s.graniteMaxHeight;
            i = 31 * i + s.dioriteSize;
            i = 31 * i + s.dioriteCount;
            i = 31 * i + s.dioriteMinHeight;
            i = 31 * i + s.dioriteMaxHeight;
            i = 31 * i + s.andesiteSize;
            i = 31 * i + s.andesiteCount;
            i = 31 * i + s.andesiteMinHeight;
            i = 31 * i + s.andesiteMaxHeight;
            i = 31 * i + s.coalSize;
            i = 31 * i + s.coalCount;
            i = 31 * i + s.coalMinHeight;
            i = 31 * i + s.coalMaxHeight;
            i = 31 * i + s.ironSize;
            i = 31 * i + s.ironCount;
            i = 31 * i + s.ironMinHeight;
            i = 31 * i + s.ironMaxHeight;
            i = 31 * i + s.goldSize;
            i = 31 * i + s.goldCount;
            i = 31 * i + s.goldMinHeight;
            i = 31 * i + s.goldMaxHeight;
            i = 31 * i + s.redstoneSize;
            i = 31 * i + s.redstoneCount;
            i = 31 * i + s.redstoneMinHeight;
            i = 31 * i + s.redstoneMaxHeight;
            i = 31 * i + s.diamondSize;
            i = 31 * i + s.diamondCount;
            i = 31 * i + s.diamondMinHeight;
            i = 31 * i + s.diamondMaxHeight;
            i = 31 * i + s.lapisSize;
            i = 31 * i + s.lapisCount;
            i = 31 * i + s.lapisCenterHeight;
            i = 31 * i + s.lapisSpread;
            return i;
        }
    };
}

void to_json(nlohmann::json& j, const ChunkGeneratorSettings::Factory& p)
{
    j["coordinateScale"] = p.coordinateScale;
    j["heightScale"] = p.heightScale;
    j["lowerLimitScale"] = p.lowerLimitScale;
    j["upperLimitScale"] = p.upperLimitScale;
    j["depthNoiseScaleX"] = p.depthNoiseScaleX;
    j["depthNoiseScaleZ"] = p.depthNoiseScaleZ;
    j["depthNoiseScaleExponent"] = p.depthNoiseScaleExponent;
    j["mainNoiseScaleX"] = p.mainNoiseScaleX;
    j["mainNoiseScaleY"] = p.mainNoiseScaleY;
    j["mainNoiseScaleZ"] = p.mainNoiseScaleZ;
    j["baseSize"] = p.baseSize;
    j["stretchY"] = p.stretchY;
    j["biomeDepthWeight"] = p.biomeDepthWeight;
    j["biomeDepthOffset"] = p.biomeDepthOffset;
    j["biomeScaleWeight"] = p.biomeScaleWeight;
    j["biomeScaleOffset"] = p.biomeScaleOffset;
    j["seaLevel"] = p.seaLevel;
    j["useCaves"] = p.useCaves;
    j["useDungeons"] = p.useDungeons;
    j["dungeonChance"] = p.dungeonChance;
    j["useStrongholds"] = p.useStrongholds;
    j["useVillages"] = p.useVillages;
    j["useMineShafts"] = p.useMineShafts;
    j["useTemples"] = p.useTemples;
    j["useMonuments"] = p.useMonuments;
    j["useMansions"] = p.useMansions;
    j["useRavines"] = p.useRavines;
    j["useWaterLakes"] = p.useWaterLakes;
    j["waterLakeChance"] = p.waterLakeChance;
    j["useLavaLakes"] = p.useLavaLakes;
    j["lavaLakeChance"] = p.lavaLakeChance;
    j["useLavaOceans"] = p.useLavaOceans;
    j["fixedBiome"] = p.fixedBiome;
    j["biomeSize"] = p.biomeSize;
    j["riverSize"] = p.riverSize;
    j["dirtSize"] = p.dirtSize;
    j["dirtCount"] = p.dirtCount;
    j["dirtMinHeight"] = p.dirtMinHeight;
    j["dirtMaxHeight"] = p.dirtMaxHeight;
    j["gravelSize"] = p.gravelSize;
    j["gravelCount"] = p.gravelCount;
    j["gravelMinHeight"] = p.gravelMinHeight;
    j["gravelMaxHeight"] = p.gravelMaxHeight;
    j["graniteSize"] = p.graniteSize;
    j["graniteCount"] = p.graniteCount;
    j["graniteMinHeight"] = p.graniteMinHeight;
    j["graniteMaxHeight"] = p.graniteMaxHeight;
    j["dioriteSize"] = p.dioriteSize;
    j["dioriteCount"] = p.dioriteCount;
    j["dioriteMinHeight"] = p.dioriteMinHeight;
    j["dioriteMaxHeight"] = p.dioriteMaxHeight;
    j["andesiteSize"] = p.andesiteSize;
    j["andesiteCount"] = p.andesiteCount;
    j["andesiteMinHeight"] = p.andesiteMinHeight;
    j["andesiteMaxHeight"] = p.andesiteMaxHeight;
    j["coalSize"] = p.coalSize;
    j["coalCount"] = p.coalCount;
    j["coalMinHeight"] = p.coalMinHeight;
    j["coalMaxHeight"] = p.coalMaxHeight;
    j["ironSize"] = p.ironSize;
    j["ironCount"] = p.ironCount;
    j["ironMinHeight"] = p.ironMinHeight;
    j["ironMaxHeight"] = p.ironMaxHeight;
    j["goldSize"] = p.goldSize;
    j["goldCount"] = p.goldCount;
    j["goldMinHeight"] = p.goldMinHeight;
    j["goldMaxHeight"] = p.goldMaxHeight;
    j["redstoneSize"] = p.redstoneSize;
    j["redstoneCount"] = p.redstoneCount;
    j["redstoneMinHeight"] = p.redstoneMinHeight;
    j["redstoneMaxHeight"] = p.redstoneMaxHeight;
    j["diamondSize"] = p.diamondSize;
    j["diamondCount"] = p.diamondCount;
    j["diamondMinHeight"] = p.diamondMinHeight;
    j["diamondMaxHeight"] = p.diamondMaxHeight;
    j["lapisSize"] = p.lapisSize;
    j["lapisCount"] = p.lapisCount;
    j["lapisCenterHeight"] = p.lapisCenterHeight;
    j["lapisSpread"] = p.lapisSpread;
}

void from_json(const nlohmann::json& j, ChunkGeneratorSettings::Factory& p)
{
    p.coordinateScale = j.at("coordinateScale").get<float>();
    p.heightScale = j.at("heightScale").get<float>();
    p.lowerLimitScale = j.at("lowerLimitScale").get<float>();
    p.upperLimitScale = j.at("upperLimitScale").get<float>();
    p.depthNoiseScaleX = j.at("depthNoiseScaleX").get<float>();
    p.depthNoiseScaleZ = j.at("depthNoiseScaleZ").get<float>();
    p.depthNoiseScaleExponent = j.at("depthNoiseScaleExponent").get<float>();
    p.mainNoiseScaleX = j.at("mainNoiseScaleX").get<float>();
    p.mainNoiseScaleY = j.at("mainNoiseScaleY").get<float>();
    p.mainNoiseScaleZ = j.at("mainNoiseScaleZ").get<float>();
    p.baseSize = j.at("baseSize").get<float>();
    p.stretchY = j.at("stretchY").get<float>();
    p.biomeDepthWeight = j.at("biomeDepthWeight").get<float>();
    p.biomeDepthOffset = j.at("biomeDepthOffset").get<float>();
    p.biomeScaleWeight = j.at("biomeScaleWeight").get<float>();
    p.biomeScaleOffset = j.at("biomeScaleOffset").get<float>();
    p.seaLevel = j.at("seaLevel").get<int32_t>();
    p.useCaves = j.at("useCaves").get<bool>();
    p.useDungeons = j.at("useDungeons").get<bool>();
    p.dungeonChance = j.at("dungeonChance").get<int32_t>();
    p.useStrongholds = j.at("useStrongholds").get<bool>();
    p.useVillages = j.at("useVillages").get<bool>();
    p.useMineShafts = j.at("useMineShafts").get<bool>();
    p.useTemples = j.at("useTemples").get<bool>();
    p.useMonuments = j.at("useMonuments").get<bool>();
    p.useMansions = j.at("useMansions").get<bool>();
    p.useRavines = j.at("useRavines").get<bool>();
    p.useWaterLakes = j.at("useWaterLakes").get<bool>();
    p.waterLakeChance = j.at("waterLakeChance").get<int32_t>();
    p.useLavaLakes = j.at("useLavaLakes").get<bool>();
    p.lavaLakeChance = j.at("lavaLakeChance").get<int32_t>();
    p.useLavaOceans = j.at("useLavaOceans").get<bool>();
    p.fixedBiome = j.at("fixedBiome").get<int32_t>();
    if (p.fixedBiome < 38 && p.fixedBiome >= -1) 
    {
        if (p.fixedBiome >= Biome::getIdForBiome(Biomes::HELL)) 
        {
            p.fixedBiome += 2;
        }
    }
    else 
    {
        p.fixedBiome = -1;
    }

    p.biomeSize = j.at("biomeSize").get<int32_t>();
    p.riverSize = j.at("riverSize").get<int32_t>();
    p.dirtSize = j.at("dirtSize").get<int32_t>();
    p.dirtCount = j.at("dirtCount").get<int32_t>();
    p.dirtMinHeight = j.at("dirtMinHeight").get<int32_t>();
    p.dirtMaxHeight = j.at("dirtMaxHeight").get<int32_t>();
    p.gravelSize = j.at("gravelSize").get<int32_t>();
    p.gravelCount = j.at("gravelCount").get<int32_t>();
    p.gravelMinHeight = j.at("gravelMinHeight").get<int32_t>();
    p.gravelMaxHeight = j.at("gravelMaxHeight").get<int32_t>();
    p.graniteSize = j.at("graniteSize").get<int32_t>();
    p.graniteCount = j.at("graniteCount").get<int32_t>();
    p.graniteMinHeight = j.at("graniteMinHeight").get<int32_t>();
    p.graniteMaxHeight = j.at("graniteMaxHeight").get<int32_t>();
    p.dioriteSize = j.at("dioriteSize").get<int32_t>();
    p.dioriteCount = j.at("dioriteCount").get<int32_t>();
    p.dioriteMinHeight = j.at("dioriteMinHeight").get<int32_t>();
    p.dioriteMaxHeight = j.at("dioriteMaxHeight").get<int32_t>();
    p.andesiteSize = j.at("andesiteSize").get<int32_t>();
    p.andesiteCount = j.at("andesiteCount").get<int32_t>();
    p.andesiteMinHeight = j.at("andesiteMinHeight").get<int32_t>();
    p.andesiteMaxHeight = j.at("andesiteMaxHeight").get<int32_t>();
    p.coalSize = j.at("coalSize").get<int32_t>();
    p.coalCount = j.at("coalCount").get<int32_t>();
    p.coalMinHeight = j.at("coalMinHeight").get<int32_t>();
    p.coalMaxHeight = j.at("coalMaxHeight").get<int32_t>();
    p.ironSize = j.at("ironSize").get<int32_t>();
    p.ironCount = j.at("ironCount").get<int32_t>();
    p.ironMinHeight = j.at("ironMinHeight").get<int32_t>();
    p.ironMaxHeight = j.at("ironMaxHeight").get<int32_t>();
    p.goldSize = j.at("goldSize").get<int32_t>();
    p.goldCount = j.at("goldCount").get<int32_t>();
    p.goldMinHeight = j.at("goldMinHeight").get<int32_t>();
    p.goldMaxHeight = j.at("goldMaxHeight").get<int32_t>();
    p.redstoneSize = j.at("redstoneSize").get<int32_t>();
    p.redstoneCount = j.at("redstoneCount").get<int32_t>();
    p.redstoneMinHeight = j.at("redstoneMinHeight").get<int32_t>();
    p.redstoneMaxHeight = j.at("redstoneMaxHeight").get<int32_t>();
    p.diamondSize = j.at("diamondSize").get<int32_t>();
    p.diamondCount = j.at("diamondCount").get<int32_t>();
    p.diamondMinHeight = j.at("diamondMinHeight").get<int32_t>();
    p.diamondMaxHeight = j.at("diamondMaxHeight").get<int32_t>();
    p.lapisSize = j.at("lapisSize").get<int32_t>();
    p.lapisCount = j.at("lapisCount").get<int32_t>();
    p.lapisCenterHeight = j.at("lapisCenterHeight").get<int32_t>();
    p.lapisSpread = j.at("lapisSpread").get<int32_t>();
}


