#include "GameType.h"

GameType GameType::NOT_SET(-1, "", "");
GameType GameType::SURVIVAL(0, "survival", "s");
GameType GameType::CREATIVE(1, "creative", "c");
GameType GameType::ADVENTURE(2, "adventure", "a");
GameType GameType::SPECTATOR(3, "spectator", "sp");

int32_t GameType::getID() const
{
    return id;
}

std::string GameType::getName() const
{
    return name;
}

void GameType::configurePlayerCapabilities(PlayerCapabilities &capabilities) const
{
    if (*this == CREATIVE)
    {
        capabilities.allowFlying    = true;
        capabilities.isCreativeMode = true;
        capabilities.disableDamage  = true;
    }
    else if (*this == SPECTATOR)
    {
        capabilities.allowFlying    = true;
        capabilities.isCreativeMode = false;
        capabilities.disableDamage  = true;
        capabilities.isFlying       = true;
    }
    else
    {
        capabilities.allowFlying    = false;
        capabilities.isCreativeMode = false;
        capabilities.disableDamage  = false;
        capabilities.isFlying       = false;
    }

    capabilities.allowEdit = !hasLimitedInteractions();
}

bool GameType::hasLimitedInteractions() const
{
    return *this == ADVENTURE || *this == SPECTATOR;
}

bool GameType::isCreative() const
{
    return *this == CREATIVE;
}

bool GameType::isSurvivalOrAdventure() const
{
    return *this == SURVIVAL || *this == ADVENTURE;
}

GameType GameType::getByID(int32_t idIn)
{
    return parseGameTypeWithDefault(idIn, SURVIVAL);
}

GameType GameType::parseGameTypeWithDefault(int32_t targetId, GameType fallback)
{
    for (auto gametype : value)
    {
        if (gametype->id == targetId)
        {
            return *gametype;
        }
    }

    return fallback;
}

GameType::GameType(int32_t idIn, std::string nameIn, std::string shortNameIn)
    : id(idIn), name(nameIn), shortName(shortNameIn)
{
    value.emplace_back(*this);
}

auto operator==(const GameType &lhs, const GameType &rhs) -> bool
{
    return lhs.id == rhs.id;
}
