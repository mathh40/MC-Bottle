#include "Team.h"

Team::EnumVisible::EnumVisible Team::EnumVisible::EnumVisible::ALWAYS = Team::EnumVisible::EnumVisible("always", 0);
Team::EnumVisible::EnumVisible Team::EnumVisible::EnumVisible::NEVER  = Team::EnumVisible::EnumVisible("never", 1);
Team::EnumVisible::EnumVisible Team::EnumVisible::EnumVisible::HIDE_FOR_OTHER_TEAMS =
    Team::EnumVisible::EnumVisible("hideForOtherTeams", 2);
Team::EnumVisible::EnumVisible Team::EnumVisible::EnumVisible::HIDE_FOR_OWN_TEAM =
    Team::EnumVisible::EnumVisible("hideForOwnTeam", 3);

Team::CollisionRule::CollisionRule ALWAYS               = Team::CollisionRule::CollisionRule("always", 0);
Team::CollisionRule::CollisionRule NEVER                = Team::CollisionRule::CollisionRule("never", 1);
Team::CollisionRule::CollisionRule HIDE_FOR_OTHER_TEAMS = Team::CollisionRule::CollisionRule("pushOtherTeams", 2);
Team::CollisionRule::CollisionRule HIDE_FOR_OWN_TEAM    = Team::CollisionRule::CollisionRule("pushOwnTeam", 3);

bool Team::isSameTeam(Team *team)
{
    if (team == nullptr)
    {
        return false;
    }
    else
    {
        return this == team;
    }
}

std::vector<std::string> Team::EnumVisible::getNames()
{
    std::vector<std::string> temp;
    for (auto pair : nameMap)
    {
        temp.emplace_back(pair.first);
    }

    return temp;
}

Team::EnumVisible Team::EnumVisible::getByName(std::string_view nameIn)
{
    return nameMap.at(nameIn);
}

Team::EnumVisible::EnumVisible(std::string_view nameIn, int32_t idIn) : internalName(nameIn), id(idIn)
{
    nameMap.emplace(nameIn, this);
}

std::vector<std::string> Team::CollisionRule::getNames()
{
    std::vector<std::string> temp;
    for (auto pair : nameMap)
    {
        temp.emplace_back(pair.first);
    }

    return temp;
}

Team::CollisionRule Team::CollisionRule::getByName(std::string_view nameIn)
{
    return nameMap.at(nameIn);
}

Team::CollisionRule::CollisionRule(std::string_view nameIn, int32_t idIn) : name(nameIn), id(idIn)
{
    nameMap.emplace(nameIn, this);
}

bool operator==(const Team &lhs, const Team &other)
{
    return lhs == other;
}
