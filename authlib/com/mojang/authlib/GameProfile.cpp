#include "GameProfile.h"

#include "StringUtils.h"

#include <fmt/core.h>

GameProfile::GameProfile(xg::Guid idIn, std::string_view nameIn)
{
    if (!idIn.isValid() && StringUtils::isBlank(nameIn))
    {
        throw std::logic_error("Name and ID cannot both be blank");
    }
    else
    {
        id   = idIn;
        name = nameIn;
    }
}

xg::Guid GameProfile::getId() const
{
    return id;
}

std::string GameProfile::getName() const
{
    return name;
}

std::unordered_map<> GameProfile::getProperties()
{
    return properties;
}

bool GameProfile::isComplete() const
{
    return id.isValid() && StringUtils::isNotBlank(getName());
}

std::string GameProfile::toString()
{
    return fmt::format("GameProfile[id={},name={},properties={},legacy={}]", id, name, properties, legacy);
}

bool GameProfile::isLegacy() const
{
    return legacy;
}

bool operator==(const GameProfile &lhs, const GameProfile &rhs)
{
    if (lhs.id.isValid())
    {
        if (!(lhs.id == rhs.id))
        {
            return false;
        }
    }
    else if (rhs.id.isValid())
    {
        return false;
    }

    if (!lhs.name.empty())
    {
        if (lhs.name == rhs.name)
        {
            return true;
        }
    }
    else if (rhs.name.empty())
    {
        return true;
    }

    return false;
}

bool operator!=(const GameProfile &lhs, const GameProfile &rhs)
{
    return !(lhs == rhs);
}
