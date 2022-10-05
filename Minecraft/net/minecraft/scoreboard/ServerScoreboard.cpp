#include "ServerScoreboard.h"

#include "ScoreObjective.h"
#include "ScorePlayerTeam.h"

ServerScoreboard::ServerScoreboard(MinecraftServer *mcServer) : server(mcServer)
{
}

void ServerScoreboard::onScoreUpdated(Score scoreIn)
{
    Scoreboard::onScoreUpdated(scoreIn);
    if (addedObjectives.contains(scoreIn.getObjective()))
    {
        server->getPlayerList().sendPacketToAllPlayers(SPacketUpdateScore(scoreIn));
    }

    markSaveDataDirty();
}

void ServerScoreboard::broadcastScoreUpdate(std::string_view scoreName)
{
    Scoreboard::broadcastScoreUpdate(scoreName);
    server->getPlayerList().sendPacketToAllPlayers(SPacketUpdateScore(scoreName));
    markSaveDataDirty();
}

void ServerScoreboard::broadcastScoreUpdate(std::string_view scoreName, ScoreObjective objective)
{
    Scoreboard::broadcastScoreUpdate(scoreName, objective);
    server->getPlayerList().sendPacketToAllPlayers(SPacketUpdateScore(scoreName, objective));
    markSaveDataDirty();
}

void ServerScoreboard::setObjectiveInDisplaySlot(int32_t objectiveSlot, ScoreObjective objective)
{
    ScoreObjective scoreobjective = getObjectiveInDisplaySlot(objectiveSlot);
    Scoreboard::setObjectiveInDisplaySlot(objectiveSlot, objective);
    if (scoreobjective != objective && scoreobjective != nullptr)
    {
        if (getObjectiveDisplaySlotCount(scoreobjective) > 0)
        {
            server->getPlayerList().sendPacketToAllPlayers(SPacketDisplayObjective(objectiveSlot, objective));
        }
        else
        {
            sendDisplaySlotRemovalPackets(scoreobjective);
        }
    }

    if (objective != nullptr)
    {
        if (addedObjectives.contains(objective))
        {
            server->getPlayerList().sendPacketToAllPlayers(SPacketDisplayObjective(objectiveSlot, objective));
        }
        else
        {
            addObjective(objective);
        }
    }

    markSaveDataDirty();
}

bool ServerScoreboard::addPlayerToTeam(std::string_view player, std::string_view newTeam)
{
    if (Scoreboard::addPlayerToTeam(player, newTeam))
    {
        ScorePlayerTeam scoreplayerteam = getTeam(newTeam);
        server->getPlayerList().sendPacketToAllPlayers(SPacketTeams(scoreplayerteam, Arrays.asList(player), 3));
        markSaveDataDirty();
        return true;
    }
    else
    {
        return false;
    }
}

void ServerScoreboard::removePlayerFromTeam(std::string_view username, ScorePlayerTeam playerTeam)
{
    Scoreboard::removePlayerFromTeam(username, playerTeam);
    server->getPlayerList().sendPacketToAllPlayers(SPacketTeams(playerTeam, Arrays.asList(username), 4));
    markSaveDataDirty();
}

void ServerScoreboard::onScoreObjectiveAdded(ScoreObjective scoreObjectiveIn)
{
    Scoreboard::onScoreObjectiveAdded(scoreObjectiveIn);
    markSaveDataDirty();
}

void ServerScoreboard::onObjectiveDisplayNameChanged(ScoreObjective objective)
{
    Scoreboard::onObjectiveDisplayNameChanged(objective);
    if (addedObjectives.contains(objective))
    {
        server.getPlayerList().sendPacketToAllPlayers(new SPacketScoreboardObjective(objective, 2));
    }

    markSaveDataDirty();
}

void ServerScoreboard::onScoreObjectiveRemoved(ScoreObjective objective)
{
    Scoreboard::onScoreObjectiveRemoved(objective);
    if (addedObjectives.contains(objective))
    {
        sendDisplaySlotRemovalPackets(objective);
    }

    markSaveDataDirty();
}

void ServerScoreboard::broadcastTeamCreated(ScorePlayerTeam playerTeam)
{
    Scoreboard::broadcastTeamCreated(playerTeam);
    server->getPlayerList().sendPacketToAllPlayers(SPacketTeams(playerTeam, 0));
    markSaveDataDirty();
}

void ServerScoreboard::broadcastTeamInfoUpdate(ScorePlayerTeam playerTeam)
{
    Scoreboard::broadcastTeamInfoUpdate(playerTeam);
    server->getPlayerList().sendPacketToAllPlayers(SPacketTeams(playerTeam, 2));
    markSaveDataDirty();
}

void ServerScoreboard::broadcastTeamRemove(ScorePlayerTeam playerTeam)
{
    Scoreboard::broadcastTeamRemove(playerTeam);
    server->getPlayerList().sendPacketToAllPlayers(SPacketTeams(playerTeam, 1));
    markSaveDataDirty();
}

void ServerScoreboard::addDirtyRunnable(Runnable runnable)
{
    dirtyRunnables = (Runnable[])((Runnable[])Arrays.copyOf(dirtyRunnables, dirtyRunnables.size() + 1));
    dirtyRunnables[dirtyRunnables.size() - 1] = runnable;
}

std::vector<> ServerScoreboard::getCreatePackets(ScoreObjective objective)
{
    std::vector<> list;
    list.add(SPacketScoreboardObjective(objective, 0));

    for (int i = 0; i < 19; ++i)
    {
        if (getObjectiveInDisplaySlot(i) == objective)
        {
            list.add(SPacketDisplayObjective(i, objective));
        }
    }

    for (auto score : getSortedScores(objective))
    {
        list.add(SPacketUpdateScore(score));
    }

    return list;
}

void ServerScoreboard::addObjective(ScoreObjective objective)
{
    std::vector<> list = getCreatePackets(objective);
    Iterator var3      = server.getPlayerList().getPlayers().iterator();

    while (var3.hasNext())
    {
        EntityPlayerMP entityplayermp = (EntityPlayerMP)var3.next();
        Iterator var5                 = list.iterator();

        while (var5.hasNext())
        {
            Packet packet = (Packet)var5.next();
            entityplayermp.connection.sendPacket(packet);
        }
    }

    addedObjectives.add(objective);
}

void ServerScoreboard::sendDisplaySlotRemovalPackets(ScoreObjective p_96546_1_)
{
    List list     = this.getDestroyPackets(p_96546_1_);
    Iterator var3 = this.server.getPlayerList().getPlayers().iterator();

    while (var3.hasNext())
    {
        EntityPlayerMP entityplayermp = (EntityPlayerMP)var3.next();
        Iterator var5                 = list.iterator();

        while (var5.hasNext())
        {
            Packet packet = (Packet)var5.next();
            entityplayermp.connection.sendPacket(packet);
        }
    }

    this.addedObjectives.remove(p_96546_1_);
}

int32_t ServerScoreboard::getObjectiveDisplaySlotCount(ScoreObjective p_96552_1_)
{
    int32_t i = 0;

    for (int32_t j = 0; j < 19; ++j)
    {
        if (getObjectiveInDisplaySlot(j) == p_96552_1_)
        {
            ++i;
        }
    }

    return i;
}

void ServerScoreboard::markSaveDataDirty()
{
    auto &var1 = dirtyRunnables;
    int var2   = var1.size();

    for (int var3 = 0; var3 < var2; ++var3)
    {
        Runnable runnable = var1[var3];
        runnable.run();
    }
}
