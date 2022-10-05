#include "WorldSettings.h"

WorldSettings::WorldSettings(int64_t seedIn, GameType &gameType, bool enableMapFeatures, bool hardcoreMode,
                             WorldType &worldTypeIn)
    : generatorOptions(""), seed(seedIn), gameType(gameType), mapFeaturesEnabled(enableMapFeatures),
      hardcoreEnabled(hardcoreMode), terrainType(worldTypeIn)
{
}

WorldSettings::WorldSettings(WorldInfo &info)
    : WorldSettings(info.getSeed(), info.getGameType(), info.isMapFeaturesEnabled(), info.isHardcoreModeEnabled(),
                    info.getTerrainType())
{
}

WorldSettings &WorldSettings::enableBonusChest()
{
    bonusChestEnabled = true;
    return *this;
}

WorldSettings &WorldSettings::enableCommands()
{
    commandsAllowed = true;
    return *this;
}

WorldSettings &WorldSettings::setGeneratorOptions(std::string_view options)
{
    generatorOptions = options;
    return *this;
}

bool WorldSettings::isBonusChestEnabled() const
{
    return bonusChestEnabled;
}

int64_t WorldSettings::getSeed() const
{
    return seed;
}

GameType &WorldSettings::getGameType()
{
    return gameType;
}

bool WorldSettings::getHardcoreEnabled() const
{
    return hardcoreEnabled;
}

bool WorldSettings::isMapFeaturesEnabled() const
{
    return mapFeaturesEnabled;
}

WorldType &WorldSettings::getTerrainType()
{
    return terrainType;
}

bool WorldSettings::areCommandsAllowed() const
{
    return commandsAllowed;
}

GameType WorldSettings::getGameTypeById(int32_t id)
{
    return GameType::getByID(id);
}

std::string WorldSettings::getGeneratorOptions() const
{
    return generatorOptions;
}
