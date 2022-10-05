#include "ScorePlayerTeam.h"

ScorePlayerTeam::ScorePlayerTeam(Scoreboard *scoreboardIn, std::string_view name)
    : nameTagVisibility(EnumVisible::ALWAYS), deathMessageVisibility(EnumVisible::ALWAYS), color(TextFormatting::RESET),
      collisionRule(CollisionRule::ALWAYS), scoreboard(scoreboardIn), name(name), displayName(name)
{
}

std::string ScorePlayerTeam::getName()
{
    return name;
}

std::string ScorePlayerTeam::getDisplayName() const
{
    return displayName;
}

void ScorePlayerTeam::setDisplayName(std::string_view name)
{
    if (name.empty())
    {
        throw std::logic_error("Name cannot be null");
    }
    else
    {
        displayName = name;
        scoreboard->broadcastTeamInfoUpdate(this);
    }
}

std::string ScorePlayerTeam::getPrefix() const
{
    return prefix;
}

void ScorePlayerTeam::setPrefix(std::string_view prefix)
{
    if (prefix.empty())
    {
        throw std::logic_error("Prefix cannot be null");
    }
    else
    {
        prefix = prefix;
        scoreboard->broadcastTeamInfoUpdate(this);
    }
}

std::string ScorePlayerTeam::getSuffix() const
{
    return suffix;
}

void ScorePlayerTeam::setSuffix(std::string_view suffix)
{
    suffix = suffix;
    scoreboard->broadcastTeamInfoUpdate(this);
}

std::string ScorePlayerTeam::formatString(const std::string &input)
{
    return getPrefix() + input + getSuffix();
}

bool ScorePlayerTeam::getAllowFriendlyFire()
{
    return allowFriendlyFire;
}

void ScorePlayerTeam::setAllowFriendlyFire(bool friendlyFire)
{
    allowFriendlyFire = friendlyFire;
    scoreboard->broadcastTeamInfoUpdate(this);
}

bool ScorePlayerTeam::getSeeFriendlyInvisiblesEnabled()
{
    return canSeeFriendlyInvisibles;
}

void ScorePlayerTeam::setSeeFriendlyInvisiblesEnabled(bool friendlyInvisibles)
{
    canSeeFriendlyInvisibles = friendlyInvisibles;
    scoreboard->broadcastTeamInfoUpdate(this);
}

Team::EnumVisible ScorePlayerTeam::getNameTagVisibility()
{
    return nameTagVisibility;
}

Team::EnumVisible ScorePlayerTeam::getDeathMessageVisibility()
{
    return deathMessageVisibility;
}

void ScorePlayerTeam::setNameTagVisibility(Team::EnumVisible visibility)
{
    nameTagVisibility = visibility;
    scoreboard->broadcastTeamInfoUpdate(this);
}

void ScorePlayerTeam::setDeathMessageVisibility(Team::EnumVisible visibility)
{
    deathMessageVisibility = visibility;
    scoreboard->broadcastTeamInfoUpdate(this);
}

Team::CollisionRule ScorePlayerTeam::getCollisionRule()
{
    return collisionRule;
}

void ScorePlayerTeam::setCollisionRule(Team::CollisionRule rule)
{
    collisionRule = rule;
    scoreboard->broadcastTeamInfoUpdate(this);
}

int32_t ScorePlayerTeam::getFriendlyFlags()
{
    int32_t i = 0;
    if (getAllowFriendlyFire())
    {
        i |= 1;
    }

    if (getSeeFriendlyInvisiblesEnabled())
    {
        i |= 2;
    }

    return i;
}

void ScorePlayerTeam::setColor(TextFormatting color)
{
    color = color;
}

TextFormatting ScorePlayerTeam::getColor()
{
    return color;
}

std::string formatPlayerName(Team *teamIn, const std::string &string)
{
    return teamIn == nullptr ? string : teamIn->formatString(string);
}
