#include "ScoreboardSaveData.h"

#include "NBTTagList.h"
#include "NBTTagString.h"
#include "text/TextFormatting.h"

ScoreboardSaveData::ScoreboardSaveData() : ScoreboardSaveData("scoreboard")
{
}

ScoreboardSaveData::ScoreboardSaveData(std::string_view name) : WorldSavedData(name)
{
}

void ScoreboardSaveData::setScoreboard(Scoreboard *scoreboardIn)
{
    scoreboard = scoreboardIn;
    if (delayedInitNbt != nullptr)
    {
        readFromNBT(delayedInitNbt.get());
    }
}

void ScoreboardSaveData::readFromNBT(NBTTagCompound *nbt)
{
    if (scoreboard == nullptr)
    {
        delayedInitNbt.reset(nbt);
    }
    else
    {
        readObjectives(nbt->getTagList("Objectives", 10));
        readScores(nbt->getTagList("PlayerScores", 10));
        if (nbt->hasKey("DisplaySlots", 10))
        {
            readDisplayConfig(nbt->getCompoundTag("DisplaySlots"));
        }

        if (nbt->hasKey("Teams", 9))
        {
            readTeams(nbt->getTagList("Teams", 10));
        }
    }
}

NBTTagCompound *ScoreboardSaveData::writeToNBT(NBTTagCompound *compound)
{
    if (scoreboard == nullptr)
    {
        LOGGER->warn("Tried to save scoreboard without having a scoreboard...");
        return compound;
    }
    else
    {
        compound->setTag("Objectives", objectivesToNbt());
        compound->setTag("PlayerScores", scoresToNbt());
        compound->setTag("Teams", teamsToNbt());
        fillInDisplaySlots(compound);
        return compound;
    }
}

void ScoreboardSaveData::readTeams(NBTTagList *tagList)
{
    for (int i = 0; i < tagList.tagCount(); ++i)
    {
        NBTTagCompound nbttagcompound = tagList.getCompoundTagAt(i);
        String s                      = nbttagcompound.getString("Name");
        if (s.length() > 16)
        {
            s = s.substring(0, 16);
        }

        ScorePlayerTeam scoreplayerteam = scoreboard.createTeam(s);
        String s1                       = nbttagcompound.getString("DisplayName");
        if (s1.length() > 32)
        {
            s1 = s1.substring(0, 32);
        }

        scoreplayerteam.setDisplayName(s1);
        if (nbttagcompound.hasKey("TeamColor", 8))
        {
            scoreplayerteam.setColor(TextFormatting.getValueByName(nbttagcompound.getString("TeamColor")));
        }

        scoreplayerteam.setPrefix(nbttagcompound.getString("Prefix"));
        scoreplayerteam.setSuffix(nbttagcompound.getString("Suffix"));
        if (nbttagcompound.hasKey("AllowFriendlyFire", 99))
        {
            scoreplayerteam.setAllowFriendlyFire(nbttagcompound.getBoolean("AllowFriendlyFire"));
        }

        if (nbttagcompound.hasKey("SeeFriendlyInvisibles", 99))
        {
            scoreplayerteam.setSeeFriendlyInvisiblesEnabled(nbttagcompound.getBoolean("SeeFriendlyInvisibles"));
        }

        Team.EnumVisible team$enumvisible1;
        if (nbttagcompound.hasKey("NameTagVisibility", 8))
        {
            team$enumvisible1 = Team.EnumVisible.getByName(nbttagcompound.getString("NameTagVisibility"));
            if (team$enumvisible1 != nullptr)
            {
                scoreplayerteam.setNameTagVisibility(team$enumvisible1);
            }
        }

        if (nbttagcompound.hasKey("DeathMessageVisibility", 8))
        {
            team$enumvisible1 = Team.EnumVisible.getByName(nbttagcompound.getString("DeathMessageVisibility"));
            if (team$enumvisible1 != nullptr)
            {
                scoreplayerteam.setDeathMessageVisibility(team$enumvisible1);
            }
        }

        if (nbttagcompound.hasKey("CollisionRule", 8))
        {
            Team.CollisionRule team$collisionrule =
                Team.CollisionRule.getByName(nbttagcompound.getString("CollisionRule"));
            if (team$collisionrule != nullptr)
            {
                scoreplayerteam.setCollisionRule(team$collisionrule);
            }
        }

        loadTeamPlayers(scoreplayerteam, nbttagcompound.getTagList("Players", 8));
    }
}

void ScoreboardSaveData::loadTeamPlayers(ScorePlayerTeam playerTeam, NBTTagList *tagList)
{
    for (int32_t i = 0; i < tagList->tagCount(); ++i)
    {
        scoreboard->addPlayerToTeam(tagList->getStringTagAt(i), playerTeam.getName());
    }
}

void ScoreboardSaveData::readDisplayConfig(NBTTagCompound *compound)
{
    for (int i = 0; i < 19; ++i)
    {
        if (compound->hasKey("slot_" + i, 8))
        {
            auto s                        = compound->getString("slot_" + i);
            ScoreObjective scoreobjective = scoreboard->getObjective(s);
            scoreboard->setObjectiveInDisplaySlot(i, scoreobjective);
        }
    }
}

void ScoreboardSaveData::readObjectives(NBTTagList *nbt)
{
    for (int i = 0; i < nbt->tagCount(); ++i)
    {
        NBTTagCompound *nbttagcompound = nbt->getCompoundTagAt(i);
        IScoreCriteria *iscorecriteria = IScoreCriteria::INSTANCES.get(nbttagcompound->getString("CriteriaName"));
        if (iscorecriteria != nullptr)
        {
            auto s = nbttagcompound->getString("Name");
            if (s.length() > 16)
            {
                s = s.substring(0, 16);
            }

            ScoreObjective scoreobjective = scoreboard.addScoreObjective(s, iscorecriteria);
            scoreobjective.setDisplayName(nbttagcompound->getString("DisplayName"));
            scoreobjective.setRenderType(
                IScoreCriteria::EnumRenderType.getByName(nbttagcompound->getString("RenderType")));
        }
    }
}

void ScoreboardSaveData::readScores(NBTTagList *nbt)
{
    for (int i = 0; i < nbt->tagCount(); ++i)
    {
        NBTTagCompound *nbttagcompound = nbt->getCompoundTagAt(i);
        auto scoreobjective            = scoreboard->getObjective(nbttagcompound->getString("Objective"));
        auto s                         = nbttagcompound->getString("Name");
        if (s.length() > 40)
        {
            s = s.substring(0, 40);
        }

        Score score = scoreboard->getOrCreateScore(s, scoreobjective);
        score.setScorePoints(nbttagcompound->getInteger("Score"));
        if (nbttagcompound->hasKey("Locked"))
        {
            score.setLocked(nbttagcompound->getBoolean("Locked"));
        }
    }
}

NBTTagList *ScoreboardSaveData::teamsToNbt()
{
    NBTTagList nbttaglist = new NBTTagList();
    Iterator var2         = scoreboard.getTeams().iterator();

    while (var2.hasNext())
    {
        ScorePlayerTeam scoreplayerteam = (ScorePlayerTeam)var2.next();
        NBTTagCompound *nbttagcompound  = new NBTTagCompound();
        nbttagcompound->setString("Name", scoreplayerteam.getName());
        nbttagcompound->setString("DisplayName", scoreplayerteam.getDisplayName());
        if (scoreplayerteam.getColor().getColorIndex() >= 0)
        {
            nbttagcompound->setString("TeamColor", scoreplayerteam.getColor().getFriendlyName());
        }

        nbttagcompound->setString("Prefix", scoreplayerteam.getPrefix());
        nbttagcompound->setString("Suffix", scoreplayerteam.getSuffix());
        nbttagcompound->setBoolean("AllowFriendlyFire", scoreplayerteam.getAllowFriendlyFire());
        nbttagcompound->setBoolean("SeeFriendlyInvisibles", scoreplayerteam.getSeeFriendlyInvisiblesEnabled());
        nbttagcompound->setString("NameTagVisibility", scoreplayerteam.getNameTagVisibility().internalName);
        nbttagcompound->setString("DeathMessageVisibility", scoreplayerteam.getDeathMessageVisibility().internalName);
        nbttagcompound->setString("CollisionRule", scoreplayerteam.getCollisionRule().name);
        NBTTagList *nbttaglist1 = new NBTTagList();
        Iterator var6           = scoreplayerteam.getMembershipCollection().iterator();

        while (var6.hasNext())
        {
            String s = (String)var6.next();
            nbttaglist1->appendTag(new NBTTagString(s));
        }

        nbttagcompound->setTag("Players", nbttaglist1);
        nbttaglist.appendTag(nbttagcompound);
    }

    return nbttaglist;
}

void ScoreboardSaveData::fillInDisplaySlots(NBTTagCompound *compound)
{
    NBTTagCompound *nbttagcompound = new NBTTagCompound();
    bool flag                      = false;

    for (int i = 0; i < 19; ++i)
    {
        ScoreObjective scoreobjective = scoreboard->getObjectiveInDisplaySlot(i);
        if (scoreobjective != nullptr)
        {
            nbttagcompound->setString("slot_" + i, scoreobjective.getName());
            flag = true;
        }
    }

    if (flag)
    {
        compound->setTag("DisplaySlots", nbttagcompound);
    }
}

NBTTagList *ScoreboardSaveData::objectivesToNbt()
{
    NBTTagList *nbttaglist = new NBTTagList();
    Iterator var2          = scoreboard->getScoreObjectives().iterator();

    while (var2.hasNext())
    {
        ScoreObjective scoreobjective = (ScoreObjective)var2.next();
        if (scoreobjective.getCriteria() != nullptr)
        {
            NBTTagCompound *nbttagcompound = new NBTTagCompound();
            nbttagcompound->setString("Name", scoreobjective.getName());
            nbttagcompound->setString("CriteriaName", scoreobjective.getCriteria().getName());
            nbttagcompound->setString("DisplayName", scoreobjective.getDisplayName());
            nbttagcompound->setString("RenderType", scoreobjective.getRenderType().getRenderType());
            nbttaglist->appendTag(nbttagcompound);
        }
    }

    return nbttaglist;
}

NBTTagList *ScoreboardSaveData::scoresToNbt()
{
    NBTTagList *nbttaglist = new NBTTagList();
    Iterator var2          = scoreboard.getScores().iterator();

    while (var2.hasNext())
    {
        Score score = (Score)var2.next();
        if (score.getObjective() != nullptr)
        {
            NBTTagCompound *nbttagcompound = new NBTTagCompound();
            nbttagcompound->setString("Name", score.getPlayerName());
            nbttagcompound->setString("Objective", score.getObjective().getName());
            nbttagcompound->setInteger("Score", score.getScorePoints());
            nbttagcompound->setBoolean("Locked", score.isLocked());
            nbttaglist->appendTag(nbttagcompound);
        }
    }

    return nbttaglist;
}
