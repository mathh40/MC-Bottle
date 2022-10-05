#include "BossInfoServer.h"

BossInfoServer::BossInfoServer(std::unique_ptr<ITextComponent> nameIn, Color colorIn, Overlay overlayIn)
    : BossInfo(xg::newGuid(), std::move(nameIn), colorIn, overlayIn), readOnlyPlayers(players), visible(true)
{
}

void BossInfoServer::setPercent(float percentIn)
{
    if (percentIn != percent)
    {
        BossInfo::setPercent(percentIn);
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_PCT);
    }
}

void BossInfoServer::setColor(Color colorIn)
{
    if (colorIn != color)
    {
        BossInfo::setColor(colorIn);
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_STYLE);
    }
}

void BossInfoServer::setOverlay(Overlay overlayIn)
{
    if (overlayIn != overlay)
    {
        BossInfo::setOverlay(overlayIn);
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_STYLE);
    }
}

BossInfo &BossInfoServer::setDarkenSky(bool darkenSkyIn)
{
    if (darkenSkyIn != darkenSky)
    {
        BossInfo::setDarkenSky(darkenSkyIn);
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_PROPERTIES);
    }

    return *this;
}

BossInfo &BossInfoServer::setPlayEndBossMusic(bool playEndBossMusicIn)
{
    if (playEndBossMusicIn != playEndBossMusic)
    {
        BossInfo::setPlayEndBossMusic(playEndBossMusicIn);
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_PROPERTIES);
    }

    return *this;
}

BossInfo &BossInfoServer::setCreateFog(bool createFogIn)
{
    if (createFogIn != createFog)
    {
        BossInfo::setCreateFog(createFogIn);
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_PROPERTIES);
    }

    return *this;
}

void BossInfoServer::setName(std::unique_ptr<ITextComponent> nameIn)
{
    if (nameIn == name)
    {
        BossInfo::setName(std::move(nameIn));
        sendUpdate(SPacketUpdateBossInfo.Operation.UPDATE_NAME);
    }
}

void BossInfoServer::sendUpdate(SPacketUpdateBossInfo.Operation operationIn)
{
    if (visible)
    {
        for (auto entityplayermp : players)
        {
            entityplayermp.connection.sendPacket(spacketupdatebossinfo);
        }
    }
}

void BossInfoServer::addPlayer(EntityPlayerMP *player)
{
    if (players.emplace(player) && visible)
    {
        player->connection.sendPacket(SPacketUpdateBossInfo(SPacketUpdateBossInfo.Operation.ADD, this));
    }
}

void BossInfoServer::removePlayer(EntityPlayerMP *player)
{
    if (players.erase(player) && visible)
    {
        player->connection.sendPacket(SPacketUpdateBossInfo(SPacketUpdateBossInfo.Operation.REMOVE, this));
    }
}

void BossInfoServer::setVisible(bool visibleIn)
{
    if (visibleIn != visible)
    {
        visible = visibleIn;

        for (auto entityplayermp : players)
        {
            entityplayermp->connection.sendPacket(SPacketUpdateBossInfo(
                visibleIn ? SPacketUpdateBossInfo.Operation.ADD : SPacketUpdateBossInfo.Operation.REMOVE, this));
        }
    }
}

std::vector<EntityPlayerMP *> BossInfoServer::getPlayers()
{
    return std::vector<EntityPlayerMP *>(readOnlyPlayers.begin(), readOnlyPlayers.end());
}
