#pragma once
#include <unordered_map>
#include <vector>

class FlatGeneratorInfo
{
  public:
    int32_t getBiome() const;
    void setBiome(int32_t biome);
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> &getWorldFeatures();
    std::vector<FlatLayerInfo> &getFlatLayers();
    void updateLayers();
    std::string toString();
    FlatGeneratorInfo createFlatGeneratorFromString(std::string_view flatGeneratorSettings);
    FlatGeneratorInfo getDefaultFlatGenerator();

  private:
    std::vector<FlatLayerInfo> flatLayers;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> worldFeatures;
    int32_t biomeToUse;

    FlatLayerInfo getLayerFromString(int32_t p_180715_0_, std::string_view p_180715_1_, int32_t p_180715_2_);
    std::vector<FlatLayerInfo> getLayersFromString(int32_t p_180716_0_, std::string_view p_180716_1_);
};
