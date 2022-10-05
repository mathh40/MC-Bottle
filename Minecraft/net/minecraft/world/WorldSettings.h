#pragma once
#include "GameType.h"

class WorldInfo;
class WorldSettings
{
  public:
    WorldSettings(int64_t seedIn, GameType &gameType, bool enableMapFeatures, bool hardcoreMode,
                  WorldType &worldTypeIn);
    explicit WorldSettings(WorldInfo &info);
    WorldSettings &enableBonusChest();
    WorldSettings &enableCommands();
    WorldSettings &setGeneratorOptions(std::string_view options);
    bool isBonusChestEnabled() const;
    int64_t getSeed() const;
    GameType &getGameType();
    bool getHardcoreEnabled() const;
    bool isMapFeaturesEnabled() const;
    WorldType &getTerrainType();
    bool areCommandsAllowed() const;
    static GameType getGameTypeById(int32_t id);
    std::string getGeneratorOptions() const;

  private:
    int64_t seed;
    GameType gameType;
    bool mapFeaturesEnabled;
    bool hardcoreEnabled;
    WorldType terrainType;
    bool commandsAllowed;
    bool bonusChestEnabled;
    std::string generatorOptions;
};
